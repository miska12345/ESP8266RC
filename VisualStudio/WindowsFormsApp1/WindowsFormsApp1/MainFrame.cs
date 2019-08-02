using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Net.Sockets;
using System.Net.NetworkInformation;

namespace WindowsFormsApp1
{
    public partial class MainFrame : MetroFramework.Forms.MetroForm
    {
        
        // Instructions
        private readonly static String INS_FORWARD_PRESSED = "FORWARD_P";
        private readonly static String INS_BACKWARED_PRESSED = "BACKWARD_P";
        private readonly static String INS_LEFT_PRESSED = "LEFT_P";
        private readonly static String INS_LEFT_RELEASED = "LEFT_R";
        private readonly static String INS_RIGHT_PRESSED = "RIGHT_P";
        private readonly static String INS_RIGHT_RELEASED = "RIGHT_R";
        private readonly static String INS_NULL = "STOP";

        private readonly static String INS_HORN_PRESSED = "HORN_P";
        private readonly static String INS_HORN_RELEASED = "HORN_R";

        private String server;
        private int port;
        
        /// <summary>
        /// This function is called automatically upon the construction of MainFrame object
        /// </summary>
        /// <param name="ip">The IP address that user have inputed</param>
        /// <param name="remote_port">The port number that the user have inputed</param>
        public MainFrame(String ip, int remote_port)
        {
            InitializeComponent();
            
            // We just store it in field variables
            // A connection will be established after we load MainFrame
            server = ip;
            port = remote_port;
        }

        /// <summary>
        /// This function is automatically called when the MainFrame window have finished loading.
        /// </summary>
        /// <param name="sender">C#'s Event param1</param>
        /// <param name="e">C#'s Event param2</param>
        private void MainFrame_Load(object sender, EventArgs e)
        {
            // Check if the remote host is online
            if (!Helper.PingHost(server))
            {
                // Not online
                MetroFramework.MetroMessageBox.Show(this, "Remote address is not responding");
            } else
            {
                // Online
                // We'll use our helper methods defined in Helper.cs to establish a connection
                if (Helper.Start(server, port))
                {
                    label_server.Text = server;
                    label_status.Text = "Connected";
                } else
                {
                    MetroFramework.MetroMessageBox.Show(this, "Connection cannot be established");
                    label_server.Text = "null";
                    label_status.Text = "Disconnected";
                }
            }
            
        }

        /// <summary>
        /// This function is called automatically when a key has been pressed
        /// </summary>
        /// <param name="sender">C#'s Event param1</param>
        /// <param name="e">C#'s Event param2</param>
        private void MainFrame_KeyDown(object sender, KeyEventArgs e)
        {
            // In this callback function, e.KeyCode holds the key pressed
            // We check if the key pressed match any of our known hot keys
            // If so, we execute a send instruction for the RC car.
            if (e.KeyCode == Keys.W)
            {
                Helper.Send(INS_FORWARD_PRESSED);
            } else if (e.KeyCode == Keys.S)
            {
                Helper.Send(INS_BACKWARED_PRESSED);
            } else if (e.KeyCode == Keys.D)
            {
                Helper.Send(INS_RIGHT_PRESSED);
            } else if (e.KeyCode == Keys.A)
            {
                Helper.Send(INS_LEFT_PRESSED);
            } else if (e.KeyCode == Keys.F)
            {
                Helper.Send(INS_HORN_PRESSED);
            }
        }

        /// <summary>
        /// This function is called when a key has been released
        /// </summary>
        /// <param name="sender">C# Event param1</param>
        /// <param name="e">C# Event param2</param>
        private void MainFrame_KeyUp(object sender, KeyEventArgs e)
        {
            // As before, we check if the key being released match any of our known hot keys
            // If so, we let the RC car know what has happened.
            if (e.KeyCode == Keys.W || e.KeyCode == Keys.S)
            {
                Helper.Send(INS_NULL);
            } else if (e.KeyCode == Keys.D)
            {
                Helper.Send(INS_RIGHT_RELEASED);
            } else if (e.KeyCode == Keys.A)
            {
                Helper.Send(INS_LEFT_RELEASED);
            }  else if (e.KeyCode == Keys.F)
            {
                Helper.Send(INS_HORN_RELEASED);
            }

        }

        /// <summary>
        /// This function is automatically called when a user clicks on the button
        /// In this case, MetroButton1 refers to the "Disconnect" button.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void MetroButton1_Click(object sender, EventArgs e)
        {
            Helper.Close();
            label_server.Text = "null";
            label_status.Text = "Disconnected";
        }

        /// <summary>
        /// This function is automatically called when a user clicks on the button
        /// In this case, MetroButton1 refers to the "Extension" button.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void MetroButton7_Click(object sender, EventArgs e)
        {
            Extension form = new Extension();
            form.ShowDialog();
        }

        /// <summary>
        /// This function is automatically called when the MainFrame window is about to exit
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void MainFrame_FormClosing(object sender, FormClosingEventArgs e)
        {
            // We disconnect from the host because this window is about to close
            MetroButton1_Click(null, null);
        }
    }
}
