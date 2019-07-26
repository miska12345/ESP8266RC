using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Threading;

namespace WindowsFormsApp1
{
    public partial class ScanNearby : MetroFramework.Forms.MetroForm
    {
        public ScanNearby()
        {
            InitializeComponent();
        }

        private void ScanNearby_Load(object sender, EventArgs e)
        {
            List<String> lst = new List<String>();
            Helper.ScanNearby(lst);
            foreach (String ip in lst)
            {
                listView1.Items.Add(ip);
            }

        }

        private void Button_cancel_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void Button_connect_Click(object sender, EventArgs e)
        {
            if (listView1.SelectedItems.Count > 0)
            {
                MainFrame window = new MainFrame(listView1.SelectedItems[0].Text, 12345);
                window.Closed += (s, args) => this.Close();
                window.Show();
                this.Hide();
            }
            

        }
    }
}
