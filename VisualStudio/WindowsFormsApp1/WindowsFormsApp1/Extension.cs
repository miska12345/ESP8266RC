using System;
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
    public partial class Extension : MetroFramework.Forms.MetroForm
    {
        public Extension()
        {
            InitializeComponent();
        }

        private void Extension_Load(object sender, EventArgs e)
        {
            metroToggle1.Checked = Global.HornEnabled;
            metroToggle2.Checked = Global.LightEnabled;
            metroToggle3.Checked = Global.SpeedEnabled;
            metroToggle4.Checked = Global.RecordEnabled;
        }

        private void MetroButton2_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void MetroButton1_Click(object sender, EventArgs e)
        {
            Global.HornEnabled = metroToggle1.Checked;
            Global.LightEnabled = metroToggle2.Checked;
            Global.SpeedEnabled = metroToggle3.Checked;
            Global.RecordEnabled = metroToggle4.Checked;
            this.Close();
        }
    }
}
