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
using System.Text.RegularExpressions;

namespace TriviaWindowsTests
{
    /// <summary>
    /// Interaction logic for CreateRoomWindow.xaml
    /// </summary>
    public partial class CreateRoomWindow : Window
    {
        private string _userName;
        private NetworkStream clientStream;
        private MenuWindow menu;
        private static OwnerRoom ownerRoomWindow;
        private static bool createOwnerRoomOnce;
        public CreateRoomWindow(string username, NetworkStream clientStream, MenuWindow menu)
        {
            InitializeComponent();
            this._userName = username;
            this.clientStream = clientStream;
            this.UserName.Text = "Logged: " + username;
            this.menu = menu;
            createOwnerRoomOnce = false;
            
        }
        private void BackToMenuWindow(object sender, RoutedEventArgs e) 
        {
            menu.Show();
            this.Hide();
        }
        private void windowClosed(object sender, EventArgs e)
        {
            menu.Close();
            if (ownerRoomWindow != null)
            { 
                ownerRoomWindow.TurnThreadOff();
                ownerRoomWindow.Close();
            }
            this.Close();
        }
        private void CharCounter_1(object sender, RoutedEventArgs e) 
        {
            Counter1.Text = RoomName_Data.Text.Length + "/" + RoomName_Data.MaxLength;
        }
        private void CharCounter_2(object sender, RoutedEventArgs e)
        {
            Counter2.Text = NumberOfPlayers_Box.Text.Length + "/" + NumberOfPlayers_Box.MaxLength;
        }
        private void CharCounter_3(object sender, RoutedEventArgs e)
        {
            Counter3.Text = NumberOfQuestions_Box.Text.Length + "/" + NumberOfQuestions_Box.MaxLength;
        }
        private void CharCounter_4(object sender, RoutedEventArgs e)
        {
            Counter4.Text = TimeForQuestion_Box.Text.Length + "/" + TimeForQuestion_Box.MaxLength;
        }
        private void NumberValidationTextBox(object sender, TextCompositionEventArgs e)
        {
            Regex regex = new Regex("[^0-9]+");
            e.Handled = regex.IsMatch(e.Text);
        }

        private void CreateRoom(object sender, RoutedEventArgs e)
        {
            HelpfulFunctions functions = new HelpfulFunctions();
            if (RoomName_Data.Text.Length > 0 && NumberOfPlayers_Box.Text.Length > 0 && NumberOfQuestions_Box.Text.Length > 0 && int.Parse(NumberOfQuestions_Box.Text) < 51 && int.Parse(NumberOfQuestions_Box.Text) > 0 && TimeForQuestion_Box.Text.Length > 0)
            {
                CreateRoomInfo roomInfo = new CreateRoomInfo(RoomName_Data.Text, Convert.ToUInt32(NumberOfPlayers_Box.Text), Convert.ToUInt32(NumberOfQuestions_Box.Text), Convert.ToUInt32(TimeForQuestion_Box.Text));
                string createRoomJson = JsonConvert.SerializeObject(roomInfo);
                byte[] requestBuffer = functions.BuildRequestBuffer(createRoomJson, Constants.createRoomRequest);
                functions.SendMsg(clientStream, requestBuffer);
                int code = functions.GetMsgCode(clientStream);
                string serverJsonRespone = functions.getServerJsonRespone(clientStream);

                if (code == Constants.createRoomRespone)
                {
                    Status respone = JsonConvert.DeserializeObject<Status>(serverJsonRespone);
                    if (respone.status == Constants.createRoomSuccessStatus)
                    {
                        MessageBox.Show("Room Created!");
                        if (createOwnerRoomOnce)
                        {
                            ownerRoomWindow.SetRoomSettings(roomInfo);
                            ownerRoomWindow.TurnWaitingThreadOn();
                        }
                        if (!createOwnerRoomOnce)
                        {
                            ownerRoomWindow = new OwnerRoom(this._userName, clientStream, this, roomInfo);
                        }
                        ownerRoomWindow.Show();
                        this.Hide();
                    }
                    else if (respone.status == Constants.createRoomFailureStatus)
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
            else 
            {
                MessageBox.Show("Room Data Is not Valid");
            }
        }

    }
}
