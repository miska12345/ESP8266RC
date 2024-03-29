﻿using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace WindowsFormsApp1
{
    public partial class Form1 : MetroFramework.Forms.MetroForm
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void MetroTile1_MouseClick(object sender, MouseEventArgs e)
        {
            ServerSelect window = new ServerSelect();
            window.Show();
        }

        private void MetroTile2_Click(object sender, EventArgs e)
        {
            ScanNearby window = new ScanNearby();
            window.Show();
        }
    }
}
