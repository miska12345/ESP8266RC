namespace WindowsFormsApp1
{
    partial class Extension
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.metroToggle1 = new MetroFramework.Controls.MetroToggle();
            this.metroLabel1 = new MetroFramework.Controls.MetroLabel();
            this.metroLabel2 = new MetroFramework.Controls.MetroLabel();
            this.metroToggle2 = new MetroFramework.Controls.MetroToggle();
            this.metroLabel3 = new MetroFramework.Controls.MetroLabel();
            this.metroToggle3 = new MetroFramework.Controls.MetroToggle();
            this.metroLabel4 = new MetroFramework.Controls.MetroLabel();
            this.metroToggle4 = new MetroFramework.Controls.MetroToggle();
            this.metroButton1 = new MetroFramework.Controls.MetroButton();
            this.metroButton2 = new MetroFramework.Controls.MetroButton();
            this.SuspendLayout();
            // 
            // metroToggle1
            // 
            this.metroToggle1.AutoSize = true;
            this.metroToggle1.Location = new System.Drawing.Point(82, 62);
            this.metroToggle1.Name = "metroToggle1";
            this.metroToggle1.Size = new System.Drawing.Size(80, 17);
            this.metroToggle1.TabIndex = 0;
            this.metroToggle1.Text = "Off";
            this.metroToggle1.UseSelectable = true;
            // 
            // metroLabel1
            // 
            this.metroLabel1.AutoSize = true;
            this.metroLabel1.Location = new System.Drawing.Point(27, 60);
            this.metroLabel1.Name = "metroLabel1";
            this.metroLabel1.Size = new System.Drawing.Size(41, 19);
            this.metroLabel1.TabIndex = 1;
            this.metroLabel1.Text = "Horn:";
            // 
            // metroLabel2
            // 
            this.metroLabel2.AutoSize = true;
            this.metroLabel2.Location = new System.Drawing.Point(23, 83);
            this.metroLabel2.Name = "metroLabel2";
            this.metroLabel2.Size = new System.Drawing.Size(45, 19);
            this.metroLabel2.TabIndex = 3;
            this.metroLabel2.Text = "Lights:";
            // 
            // metroToggle2
            // 
            this.metroToggle2.AutoSize = true;
            this.metroToggle2.Location = new System.Drawing.Point(82, 85);
            this.metroToggle2.Name = "metroToggle2";
            this.metroToggle2.Size = new System.Drawing.Size(80, 17);
            this.metroToggle2.TabIndex = 2;
            this.metroToggle2.Text = "Off";
            this.metroToggle2.UseSelectable = true;
            // 
            // metroLabel3
            // 
            this.metroLabel3.AutoSize = true;
            this.metroLabel3.Location = new System.Drawing.Point(20, 106);
            this.metroLabel3.Name = "metroLabel3";
            this.metroLabel3.Size = new System.Drawing.Size(49, 19);
            this.metroLabel3.TabIndex = 5;
            this.metroLabel3.Text = "Speed:";
            // 
            // metroToggle3
            // 
            this.metroToggle3.AutoSize = true;
            this.metroToggle3.Location = new System.Drawing.Point(82, 108);
            this.metroToggle3.Name = "metroToggle3";
            this.metroToggle3.Size = new System.Drawing.Size(80, 17);
            this.metroToggle3.TabIndex = 4;
            this.metroToggle3.Text = "Off";
            this.metroToggle3.UseSelectable = true;
            // 
            // metroLabel4
            // 
            this.metroLabel4.AutoSize = true;
            this.metroLabel4.Location = new System.Drawing.Point(14, 129);
            this.metroLabel4.Name = "metroLabel4";
            this.metroLabel4.Size = new System.Drawing.Size(54, 19);
            this.metroLabel4.TabIndex = 7;
            this.metroLabel4.Text = "Record:";
            // 
            // metroToggle4
            // 
            this.metroToggle4.AutoSize = true;
            this.metroToggle4.Location = new System.Drawing.Point(82, 131);
            this.metroToggle4.Name = "metroToggle4";
            this.metroToggle4.Size = new System.Drawing.Size(80, 17);
            this.metroToggle4.TabIndex = 6;
            this.metroToggle4.Text = "Off";
            this.metroToggle4.UseSelectable = true;
            // 
            // metroButton1
            // 
            this.metroButton1.Location = new System.Drawing.Point(14, 195);
            this.metroButton1.Name = "metroButton1";
            this.metroButton1.Size = new System.Drawing.Size(76, 35);
            this.metroButton1.TabIndex = 8;
            this.metroButton1.Text = "Save";
            this.metroButton1.UseSelectable = true;
            this.metroButton1.Click += new System.EventHandler(this.MetroButton1_Click);
            // 
            // metroButton2
            // 
            this.metroButton2.Location = new System.Drawing.Point(106, 195);
            this.metroButton2.Name = "metroButton2";
            this.metroButton2.Size = new System.Drawing.Size(77, 35);
            this.metroButton2.TabIndex = 9;
            this.metroButton2.Text = "Cancel";
            this.metroButton2.UseSelectable = true;
            this.metroButton2.Click += new System.EventHandler(this.MetroButton2_Click);
            // 
            // Extension
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(196, 271);
            this.Controls.Add(this.metroButton2);
            this.Controls.Add(this.metroButton1);
            this.Controls.Add(this.metroLabel4);
            this.Controls.Add(this.metroToggle4);
            this.Controls.Add(this.metroLabel3);
            this.Controls.Add(this.metroToggle3);
            this.Controls.Add(this.metroLabel2);
            this.Controls.Add(this.metroToggle2);
            this.Controls.Add(this.metroLabel1);
            this.Controls.Add(this.metroToggle1);
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "Extension";
            this.Resizable = false;
            this.ShowIcon = false;
            this.Text = "Extensions";
            this.Load += new System.EventHandler(this.Extension_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private MetroFramework.Controls.MetroToggle metroToggle1;
        private MetroFramework.Controls.MetroLabel metroLabel1;
        private MetroFramework.Controls.MetroLabel metroLabel2;
        private MetroFramework.Controls.MetroToggle metroToggle2;
        private MetroFramework.Controls.MetroLabel metroLabel3;
        private MetroFramework.Controls.MetroToggle metroToggle3;
        private MetroFramework.Controls.MetroLabel metroLabel4;
        private MetroFramework.Controls.MetroToggle metroToggle4;
        private MetroFramework.Controls.MetroButton metroButton1;
        private MetroFramework.Controls.MetroButton metroButton2;
    }
}