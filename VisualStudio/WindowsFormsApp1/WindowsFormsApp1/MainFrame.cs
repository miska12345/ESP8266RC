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
        
        public MainFrame(String ip, int remote_port)
        {
            InitializeComponent();
            
            // Establish connection
            server = ip;
            port = remote_port;
        }

        private void MainFrame_Load(object sender, EventArgs e)
        {
            if (!Helper.PingHost(server))
            {
                MetroFramework.MetroMessageBox.Show(this, "Remote address is not responding");
            } else
            {
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

        private void MainFrame_KeyDown(object sender, KeyEventArgs e)
        {
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

        private void MainFrame_KeyUp(object sender, KeyEventArgs e)
        {
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

        private void MetroButton1_Click(object sender, EventArgs e)
        {
            Helper.Close();
            label_server.Text = "null";
            label_status.Text = "Disconnected";
        }

        private void MetroButton7_Click(object sender, EventArgs e)
        {
            Extension form = new Extension();
            form.ShowDialog();
        }

        private void MainFrame_FormClosing(object sender, FormClosingEventArgs e)
        {
            MetroButton1_Click(null, null);
        }
    }
}
