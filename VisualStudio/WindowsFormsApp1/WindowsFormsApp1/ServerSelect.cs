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

namespace WindowsFormsApp1
{
    public partial class ServerSelect : MetroFramework.Forms.MetroForm
    {
        public ServerSelect()
        {
            InitializeComponent();
        }

        private void MetroButton2_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void MetroButton1_Click(object sender, EventArgs e)
        {
            String server = metroTextBox1.Text;
            if (server.Length == 0)
            {
                MetroFramework.MetroMessageBox.Show(this, "Please enter a non-empty IP address");
                return;
            }

            int port = -1;
            int.TryParse(metroTextBox2.Text, out port);
            if (port == -1)
            {
                MetroFramework.MetroMessageBox.Show(this, "Please enter a valid port number (numbers only)");
                return;
            }
            MainFrame main = new MainFrame(server, port);
            main.Closed += (s, args) => this.Close();
            main.Show();
            this.Hide();
        }

        private void ServerSelect_Load(object sender, EventArgs e)
        {

        }
    }
}
