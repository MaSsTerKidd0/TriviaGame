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

namespace TriviaWindowsTests
{
    public partial class LeaderboardWindow : Window
    {
        private string _userName;
        private NetworkStream clientStream;
        private MenuWindow menu;
        public LeaderboardWindow(string userName, NetworkStream clientStream, MenuWindow menu)
        {
            InitializeComponent();
            this._userName = userName;
            this.clientStream = clientStream;
            this.UserName.Text = "Logged: " + userName;
            this.menu = menu;
            ShowLeaderboards();
        }
        private void BackToMenuWindow(object sender, RoutedEventArgs e)
        {
            menu.Show();
            this.Hide();
        }
        private void windowClosed(object sender, EventArgs e) 
        {
            menu.Close();
            this.Close();
        }
        private void ShowLeaderboards()
        {
            HelpfulFunctions functions = new HelpfulFunctions();
            string leaderboardsJson = "";
            byte[] requestBuffer = functions.BuildRequestBuffer(leaderboardsJson, Constants.leaderboardsRequest);
            functions.SendMsg(clientStream, requestBuffer);
            int code = functions.GetMsgCode(clientStream);
            string serverJsonRespone = functions.getServerJsonRespone(clientStream);

            if (code == Constants.leaderboardsRespone)
            {
                LeaderBoardsInfo respone = JsonConvert.DeserializeObject<LeaderBoardsInfo>(serverJsonRespone);

                if (respone.status == Constants.leaderboardsSuccessStatus)
                {
                    //MessageBox.Show(FirstPlace.Text);
                    FirstPlace.Inlines.Add(FirstPlace.Text + respone.highScores[0] + " (" + respone.highScores[1] + ")");
                    SecondPlace.Inlines.Add(SecondPlace.Text + respone.highScores[2] + " (" + respone.highScores[3] + ")");
                    ThirdPlace.Inlines.Add(ThirdPlace.Text + respone.highScores[4] + " (" + respone.highScores[5] + ")");
                }
                else if (respone.status == Constants.leaderboardsFailureStatus)
                {
                    MessageBox.Show("ERROR OCCURED");
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
