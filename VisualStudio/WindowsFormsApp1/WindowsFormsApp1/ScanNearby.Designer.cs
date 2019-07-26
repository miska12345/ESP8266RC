namespace WindowsFormsApp1
{
    partial class ScanNearby
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
            this.listView1 = new MetroFramework.Controls.MetroListView();
            this.metroLabel1 = new MetroFramework.Controls.MetroLabel();
            this.button_connect = new MetroFramework.Controls.MetroButton();
            this.button_cancel = new MetroFramework.Controls.MetroButton();
            this.SuspendLayout();
            // 
            // listView1
            // 
            this.listView1.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.listView1.CheckBoxes = true;
            this.listView1.Font = new System.Drawing.Font("Segoe UI", 12F);
            this.listView1.FullRowSelect = true;
            this.listView1.Location = new System.Drawing.Point(14, 104);
            this.listView1.Name = "listView1";
            this.listView1.OwnerDraw = true;
            this.listView1.Size = new System.Drawing.Size(278, 108);
            this.listView1.Sorting = System.Windows.Forms.SortOrder.Ascending;
            this.listView1.TabIndex = 0;
            this.listView1.UseCompatibleStateImageBehavior = false;
            this.listView1.UseSelectable = true;
            this.listView1.View = System.Windows.Forms.View.List;
            // 
            // metroLabel1
            // 
            this.metroLabel1.AutoSize = true;
            this.metroLabel1.Location = new System.Drawing.Point(14, 60);
            this.metroLabel1.Name = "metroLabel1";
            this.metroLabel1.Size = new System.Drawing.Size(278, 19);
            this.metroLabel1.TabIndex = 1;
            this.metroLabel1.Text = "Select the detected remote devices to connect";
            // 
            // button_connect
            // 
            this.button_connect.Location = new System.Drawing.Point(23, 227);
            this.button_connect.Name = "button_connect";
            this.button_connect.Size = new System.Drawing.Size(85, 33);
            this.button_connect.TabIndex = 2;
            this.button_connect.Text = "Connect";
            this.button_connect.UseSelectable = true;
            this.button_connect.Click += new System.EventHandler(this.Button_connect_Click);
            // 
            // button_cancel
            // 
            this.button_cancel.Location = new System.Drawing.Point(205, 227);
            this.button_cancel.Name = "button_cancel";
            this.button_cancel.Size = new System.Drawing.Size(81, 33);
            this.button_cancel.TabIndex = 3;
            this.button_cancel.Text = "Cancel";
            this.button_cancel.UseSelectable = true;
            this.button_cancel.Click += new System.EventHandler(this.Button_cancel_Click);
            // 
            // ScanNearby
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(309, 287);
            this.Controls.Add(this.button_cancel);
            this.Controls.Add(this.button_connect);
            this.Controls.Add(this.metroLabel1);
            this.Controls.Add(this.listView1);
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "ScanNearby";
            this.Resizable = false;
            this.Text = "ScanNearby";
            this.Load += new System.EventHandler(this.ScanNearby_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private MetroFramework.Controls.MetroListView listView1;
        private MetroFramework.Controls.MetroLabel metroLabel1;
        private MetroFramework.Controls.MetroButton button_connect;
        private MetroFramework.Controls.MetroButton button_cancel;
    }
}