namespace WindowsFormsApp1
{
    partial class Form1
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form1));
            this.tile_connect = new MetroFramework.Controls.MetroTile();
            this.tile_nearby = new MetroFramework.Controls.MetroTile();
            this.tile_qr = new MetroFramework.Controls.MetroTile();
            this.tile_about = new MetroFramework.Controls.MetroTile();
            this.SuspendLayout();
            // 
            // tile_connect
            // 
            this.tile_connect.ActiveControl = null;
            this.tile_connect.Location = new System.Drawing.Point(23, 63);
            this.tile_connect.Name = "tile_connect";
            this.tile_connect.Size = new System.Drawing.Size(232, 226);
            this.tile_connect.TabIndex = 0;
            this.tile_connect.Text = "Connect to IP";
            this.tile_connect.TileImage = ((System.Drawing.Image)(resources.GetObject("tile_connect.TileImage")));
            this.tile_connect.TileImageAlign = System.Drawing.ContentAlignment.MiddleCenter;
            this.tile_connect.UseSelectable = true;
            this.tile_connect.UseTileImage = true;
            this.tile_connect.MouseClick += new System.Windows.Forms.MouseEventHandler(this.MetroTile1_MouseClick);
            // 
            // tile_nearby
            // 
            this.tile_nearby.ActiveControl = null;
            this.tile_nearby.Location = new System.Drawing.Point(261, 63);
            this.tile_nearby.Name = "tile_nearby";
            this.tile_nearby.Size = new System.Drawing.Size(267, 100);
            this.tile_nearby.Style = MetroFramework.MetroColorStyle.Brown;
            this.tile_nearby.TabIndex = 1;
            this.tile_nearby.Text = "Search Nearby";
            this.tile_nearby.UseSelectable = true;
            this.tile_nearby.Click += new System.EventHandler(this.MetroTile2_Click);
            // 
            // tile_qr
            // 
            this.tile_qr.ActiveControl = null;
            this.tile_qr.Location = new System.Drawing.Point(261, 169);
            this.tile_qr.Name = "tile_qr";
            this.tile_qr.Size = new System.Drawing.Size(127, 120);
            this.tile_qr.Style = MetroFramework.MetroColorStyle.Purple;
            this.tile_qr.TabIndex = 2;
            this.tile_qr.Text = "Scan QR Code";
            this.tile_qr.UseSelectable = true;
            // 
            // tile_about
            // 
            this.tile_about.ActiveControl = null;
            this.tile_about.Location = new System.Drawing.Point(394, 169);
            this.tile_about.Name = "tile_about";
            this.tile_about.Size = new System.Drawing.Size(134, 120);
            this.tile_about.Style = MetroFramework.MetroColorStyle.Teal;
            this.tile_about.TabIndex = 3;
            this.tile_about.Text = "About";
            this.tile_about.UseSelectable = true;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackgroundImage = ((System.Drawing.Image)(resources.GetObject("$this.BackgroundImage")));
            this.ClientSize = new System.Drawing.Size(555, 312);
            this.Controls.Add(this.tile_about);
            this.Controls.Add(this.tile_qr);
            this.Controls.Add(this.tile_nearby);
            this.Controls.Add(this.tile_connect);
            this.Name = "Form1";
            this.Text = "NodeMCU Software";
            this.ResumeLayout(false);

        }

        #endregion

        private MetroFramework.Controls.MetroTile tile_connect;
        private MetroFramework.Controls.MetroTile tile_nearby;
        private MetroFramework.Controls.MetroTile tile_qr;
        private MetroFramework.Controls.MetroTile tile_about;
    }
}

