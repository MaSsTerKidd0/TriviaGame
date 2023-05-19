using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;

using System.Net.Sockets;
using System.Net;
using Newtonsoft.Json;
using SerializeAndDeserialize;
using System.Threading;

namespace TriviaWindowsTests
{
    /// <summary>
    /// Interaction logic for JoinRoomWindow.xaml
    /// </summary>
    public partial class JoinRoomWindow : Window
    {
        private string _userName;
        private NetworkStream clientStream;
        private MenuWindow menu;
        private static WaitingRoom waitingRoomWindow;
        private static string roomToJoin = "";
        private static bool openWaitRoomOnce;
        private List<string> players;
        private static Thread showRoomsThread;
        private static ManualResetEvent _event;

        public JoinRoomWindow(string userName, NetworkStream clientStream, MenuWindow menu)
        {
            InitializeComponent();
            this._userName = userName;
            this.clientStream = clientStream;
            this.UserName.Text = "Logged: " + userName;
            this.menu = menu;
            openWaitRoomOnce = false;
            _event = new ManualResetEvent(true);
            showRoomsThread = new Thread(ShowRoomsThread);
            showRoomsThread.Start();
        }
        private void BackToMenuWindow(object sender, RoutedEventArgs e)
        {
            _event.Reset();
            menu.Show();
            this.Hide();
        }
        private void JoinRoomAndOpenWaitingRoom(object sender, RoutedEventArgs e)
        {
            if (roomToJoin != "")
            {
                CreateRoomInfo roomInfo = GetRoomSettings();
                if (!openWaitRoomOnce)
                {
                    _event.Reset();
                    JoinRoom();
                    waitingRoomWindow = new WaitingRoom(this._userName, clientStream, roomInfo, this);
                    openWaitRoomOnce = true;
                    waitingRoomWindow.Show();
                }
                else
                {
                    _event.Reset();
                    JoinRoom();
                    waitingRoomWindow.TurnWaitingThreadOn();
                    waitingRoomWindow.SetRoomSettingsAndPlayers(roomInfo);
                    waitingRoomWindow.Show();
                }
                this.Hide();
            }
        }
        private void JoinRoom()
        {
            HelpfulFunctions functions = new HelpfulFunctions();
            if (onlineRooms.SelectedItem.ToString().Length > 0)
            {
                JoinRoomRequest joinRoomRequest = new JoinRoomRequest(roomToJoin);
                string joinRoomJson = JsonConvert.SerializeObject(joinRoomRequest);//Need To Be Room Id
                byte[] requestBuffer = functions.BuildRequestBuffer(joinRoomJson, Constants.joinRoomRequest);
                functions.SendMsg(clientStream, requestBuffer);
                int code = functions.GetMsgCode(clientStream);
                string serverJsonRespone = functions.getServerJsonRespone(clientStream);

                if (code == Constants.joinRoomRespone)
                {
                    JoinRoomRespone respone = JsonConvert.DeserializeObject<JoinRoomRespone>(serverJsonRespone);
                    if (respone.status == Constants.joinRoomSuccessStatus)
                    {
                        MessageBox.Show("JoinedRoom!");
                        GetPlayers();
                    }
                    else if (respone.status == Constants.joinRoomFailureStatus)
                    {
                        MessageBox.Show("ERROR: Check the details and try again.");
                    }
                }
                else
                {
                    Error error = JsonConvert.DeserializeObject<Error>(serverJsonRespone);
                    MessageBox.Show("ERROR: " + error.message);
                }
            }
        }
        private void RefreshRooms(object sender, RoutedEventArgs e)
        {
            List<string> showRooms = ShowRooms();
            onlineRooms.ItemsSource = showRooms;
        }

        private void windowClosed(object sender, EventArgs e)
        {
            menu.Close();
            if (waitingRoomWindow != null)
            {
                waitingRoomWindow.Close();
            }
            _event.Close();
            showRoomsThread.Abort();
            this.Close();
        }
        private void ShowRoomsThread()
        {
            while (true)
            {
                _event.WaitOne();
                List<string> showRooms = ShowRooms();
                this.Dispatcher.Invoke(() =>
                {
                    onlineRooms.ItemsSource = showRooms;
                    if (roomToJoin != "")
                    {
                        GetPlayersRequest(roomToJoin);
                    }
                });
                Thread.Sleep(3000);
            }
        }
        private List<string> ShowRooms()
        {
            HelpfulFunctions functions = new HelpfulFunctions();
            string getRoomsJson = "";
            byte[] requestBuffer = functions.BuildRequestBuffer(getRoomsJson, Constants.getRoomsRequest);
            functions.SendMsg(clientStream, requestBuffer);
            int code = functions.GetMsgCode(clientStream);
            string serverJsonRespone = functions.getServerJsonRespone(clientStream);

            if (code == Constants.getRoomsRespone)
            {
                GetRooms respone = JsonConvert.DeserializeObject<GetRooms>(serverJsonRespone);
                if (respone.status == Constants.getRoomsSuccessStatus && respone.rooms != "")
                {
                    List<string> showRooms = new List<string>();
                    string[] rooms = respone.rooms.Split(',');
                    for (int i = 0; i < rooms.Length; i++)
                    {
                        showRooms.Add(rooms[i]);
                    }
                    return showRooms;
                }
                return null;
            }
            else
            {
                Error error = JsonConvert.DeserializeObject<Error>(serverJsonRespone);
                MessageBox.Show("ERROR: " + error.message);
                return null;
            }
        }
        private void ShowPlayers(object sender, SelectionChangedEventArgs args)
        {
            GetPlayers();
        }
        private void GetPlayers()
        {
            roomToJoin = onlineRooms.SelectedItem.ToString();
            GetPlayersRequest(roomToJoin);
        }
        private void GetPlayersRequest(string name) 
        {
            HelpfulFunctions functions = new HelpfulFunctions();
            List<string> showPlayers = functions.ShowPlayers(clientStream, roomToJoin);
            this.players = showPlayers.ToList();
            if (showPlayers.Count != 0)
                Selected_Room_Players.ItemsSource = showPlayers;
        }
        private CreateRoomInfo GetRoomSettings()
        {
            CreateRoomInfo roomInfo = new CreateRoomInfo("", 0, 0, 0);
            HelpfulFunctions functions = new HelpfulFunctions();
            string loginJson = JsonConvert.SerializeObject(new GetRoomSettingsRequest(roomToJoin));
            byte[] requestBuffer = functions.BuildRequestBuffer(loginJson, Constants.roomInfoRequest);
            functions.SendMsg(clientStream, requestBuffer);
            int code = functions.GetMsgCode(clientStream);
            string serverJsonRespone = functions.getServerJsonRespone(clientStream);

            if (code == Constants.roomInfoRespone)
            {
                roomInfo = JsonConvert.DeserializeObject<CreateRoomInfo>(serverJsonRespone);
            }
            else
            {
                Error error = JsonConvert.DeserializeObject<Error>(serverJsonRespone);
                MessageBox.Show("ERROR: " + error.message);
            }
            return roomInfo;
        }

        public void TurnThreadOn()
        {
            _event.Set();
        }

        private void GetRoomState()
        {
            HelpfulFunctions functions = new HelpfulFunctions();
            string getRoomStateJson = "";
            byte[] requestBuffer = functions.BuildRequestBuffer(getRoomStateJson, Constants.getRoomStateRequest);
            functions.SendMsg(clientStream, requestBuffer);
            int code = functions.GetMsgCode(clientStream);
            string serverJsonRespone = functions.getServerJsonRespone(clientStream);

            if (code == Constants.getRoomStateRespone)
            {
                RoomState respone = JsonConvert.DeserializeObject<RoomState>(serverJsonRespone);
                if (respone.status == Constants.getRoomStateSuccessStatus)
                {
                    if (respone.hasGameBegun == Constants.roomStateStart)
                    {
                        MessageBox.Show("Game Started!");
                    }
                    else if (respone.hasGameBegun == Constants.roomStateClose)
                    {
                        leaveRoom();
                    }
                    else
                    {
                        // waiting
                    }
                }
                else if (respone.status == Constants.getRoomStateFailureStatus)
                {
                    MessageBox.Show("ERROR: Get Room State Failed!");
                }
            }
            else
            {
                Error error = JsonConvert.DeserializeObject<Error>(serverJsonRespone);
                MessageBox.Show("ERROR: " + error.message);
            }
        }
        private void leaveRoom()
        {
            HelpfulFunctions functions = new HelpfulFunctions();
            string getRoomStateJson = "";
            byte[] requestBuffer = functions.BuildRequestBuffer(getRoomStateJson, Constants.leaveRoomRequest);
            functions.SendMsg(clientStream, requestBuffer);
            int code = functions.GetMsgCode(clientStream);
            string serverJsonRespone = functions.getServerJsonRespone(clientStream);

            if (code == Constants.leaveRoomRespone)
            {
                RoomState respone = JsonConvert.DeserializeObject<RoomState>(serverJsonRespone);
                if (respone.status == Constants.leaveRoomSuccessStatus)
                {
                    MessageBox.Show("Left Room!");
                }
                else if (respone.status == Constants.leaveRoomFailureStatus)
                {
                    MessageBox.Show("ERROR: Leave Room Failed!");
                }
            }
            else
            {
                Error error = JsonConvert.DeserializeObject<Error>(serverJsonRespone);
                MessageBox.Show("ERROR: " + error.message);
            }
        }
    }
}
