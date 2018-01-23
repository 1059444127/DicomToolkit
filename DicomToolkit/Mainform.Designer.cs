using System.Windows.Forms;

namespace DicomToolkit
{
    public sealed partial class Mainform
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
            this.components = new System.ComponentModel.Container();

            this.m_load_multi_frames_button = new Button();
            this.m_saved_dest_button        = new Button();
            this.m_source_path_label        = new Label();
            this.m_dest_path_label          = new Label();
            this.m_source_path_textbox      = new TextBox();
            this.m_dest_path_textbox        = new TextBox();
            this.m_run_button               = new Button();
            this.m_cancel_button            = new Button();
            this.m_progressBar              = new ProgressBar();
            this.m_author_name              = new Label();

            // Load multi frames button
            this.m_load_multi_frames_button.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.m_load_multi_frames_button.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.m_load_multi_frames_button.ForeColor = System.Drawing.Color.Black;
            this.m_load_multi_frames_button.Location = new System.Drawing.Point(450, 100);
            this.m_load_multi_frames_button.Name = "load_multi_frames";
            this.m_load_multi_frames_button.Size = new System.Drawing.Size(130, 30);
            this.m_load_multi_frames_button.TabIndex = 1;
            this.m_load_multi_frames_button.Text = "Select Dicom";
            this.m_load_multi_frames_button.UseVisualStyleBackColor = false;
            this.m_load_multi_frames_button.Click += new System.EventHandler(this.SelectDicomFile);

            // Saved frames button
            this.m_saved_dest_button.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.m_saved_dest_button.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.m_saved_dest_button.ForeColor = System.Drawing.Color.Black;
            this.m_saved_dest_button.Location = new System.Drawing.Point(450, 200);
            this.m_saved_dest_button.Name = "saved_frames_path";
            this.m_saved_dest_button.Size = new System.Drawing.Size(130, 30);
            this.m_saved_dest_button.TabIndex = 2;
            this.m_saved_dest_button.Text = "Save Dicom To";
            this.m_saved_dest_button.UseVisualStyleBackColor = false;
            this.m_saved_dest_button.Click += new System.EventHandler(this.SelectDestFolder);

            // Source path label
            this.m_source_path_label.AutoSize = false;
            this.m_source_path_label.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.0F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.m_source_path_label.Location = new System.Drawing.Point(40, 100);
            this.m_source_path_label.Name = "source_path_label";
            this.m_source_path_label.Size = new System.Drawing.Size(100, 30);
            this.m_source_path_label.TabIndex = 3;
            this.m_source_path_label.Text = "Source Path: ";
            this.m_source_path_label.Visible = true;

            // Destination path label
            this.m_dest_path_label.AutoSize = false;
            this.m_dest_path_label.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.0F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.m_dest_path_label.Location = new System.Drawing.Point(40, 200);
            this.m_dest_path_label.Name = "dest_path_label";
            this.m_dest_path_label.Size = new System.Drawing.Size(100, 30);
            this.m_dest_path_label.TabIndex = 4;
            this.m_dest_path_label.Text = "Target Path: ";
            this.m_dest_path_label.Visible = true;

            // Source path textbox
            this.m_source_path_textbox.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.m_source_path_textbox.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.0F);
            this.m_source_path_textbox.Location = new System.Drawing.Point(150, 100);
            this.m_source_path_textbox.Margin = new System.Windows.Forms.Padding(0);
            this.m_source_path_textbox.Name = "m_source_path_textbox";
            this.m_source_path_textbox.Size = new System.Drawing.Size(240, 30);
            this.m_source_path_textbox.TabIndex = 5;
            this.m_source_path_textbox.WordWrap = false;
            this.m_source_path_textbox.Enabled = true;
            this.m_source_path_textbox.Text = "";
            this.m_source_path_textbox.TextChanged += new System.EventHandler(this.UpdateSourcePath);

            // Destination path textbox
            this.m_dest_path_textbox.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.m_dest_path_textbox.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.0F);
            this.m_dest_path_textbox.Location = new System.Drawing.Point(150, 200);
            this.m_dest_path_textbox.Margin = new System.Windows.Forms.Padding(0);
            this.m_dest_path_textbox.Name = "m_dest_path_textbox";
            this.m_dest_path_textbox.Size = new System.Drawing.Size(240, 30);
            this.m_dest_path_textbox.TabIndex = 6;
            this.m_dest_path_textbox.WordWrap = false;
            this.m_dest_path_textbox.Enabled = true;
            this.m_dest_path_textbox.Text = "";
            this.m_dest_path_textbox.TextChanged += new System.EventHandler(this.UpdateTargetPath);

            // Run button
            this.m_run_button.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.m_run_button.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.m_run_button.ForeColor = System.Drawing.Color.Black;
            this.m_run_button.Location = new System.Drawing.Point(150, 300);
            this.m_run_button.Name = "Run";
            this.m_run_button.Size = new System.Drawing.Size(100, 30);
            this.m_run_button.TabIndex = 7;
            this.m_run_button.Text = "Run";
            this.m_run_button.UseVisualStyleBackColor = false;
            this.m_run_button.Enabled = false;
            this.m_run_button.Click += new System.EventHandler(this.DoRun);

            // Cancel button
            this.m_cancel_button.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.m_cancel_button.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.m_cancel_button.ForeColor = System.Drawing.Color.Black;
            this.m_cancel_button.Location = new System.Drawing.Point(350, 300);
            this.m_cancel_button.Name = "Cancel";
            this.m_cancel_button.Size = new System.Drawing.Size(100, 30);
            this.m_cancel_button.TabIndex = 8;
            this.m_cancel_button.Text = "Cancel";
            this.m_cancel_button.UseVisualStyleBackColor = false;
            this.m_cancel_button.Enabled = false;
            this.m_cancel_button.Click += new System.EventHandler(this.DoCancel);

            // Progress bar
            this.m_progressBar.Location = new System.Drawing.Point(25, 350);
            this.m_progressBar.Size = new System.Drawing.Size(550, 20);
            this.m_progressBar.Maximum = 100;
            this.m_progressBar.Value = 0;
            this.m_progressBar.Step = 5;

            // author name label
            this.m_author_name.AutoSize = false;
            this.m_author_name.Font = new System.Drawing.Font("Microsoft Sans Serif", 7.0F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.m_author_name.Location = new System.Drawing.Point(530, 385);
            this.m_author_name.Name = "author_label";
            this.m_author_name.Size = new System.Drawing.Size(70, 20);
            this.m_author_name.TabIndex = 9;
            this.m_author_name.Text = "@LF.Zhao";
            this.m_author_name.Visible = true;

            this.ClientSize = new System.Drawing.Size(600, 400);
            this.Controls.Add(this.m_load_multi_frames_button);
            this.Controls.Add(this.m_saved_dest_button);
            this.Controls.Add(this.m_source_path_label);
            this.Controls.Add(this.m_dest_path_label);
            this.Controls.Add(this.m_source_path_textbox);
            this.Controls.Add(this.m_dest_path_textbox);
            this.Controls.Add(this.m_run_button);
            this.Controls.Add(this.m_cancel_button);
            this.Controls.Add(this.m_progressBar);
            this.Controls.Add(this.m_author_name);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Text = "Dicom Toolkit";
            this.Name = "Main Form";
        }

        // Variables
        private Button   m_load_multi_frames_button;
        private Button   m_saved_dest_button;
        private Label    m_source_path_label;
        private Label    m_dest_path_label;
        private TextBox  m_source_path_textbox;
        private TextBox  m_dest_path_textbox;
        private Button   m_run_button;
        private Button   m_cancel_button;
        private ProgressBar m_progressBar;
        private Label    m_author_name;

        #endregion
    }
}

