using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO;
using System.Text.RegularExpressions;
using DicomNativeWrapper;

namespace DicomToolkit
{
    public partial class Mainform : Form
    {
        private String m_source_file_path;
        private String m_destination_foler_path;
        private DicomWrapper m_dicom_wrapper;

        public Mainform()
        {
            InitializeComponent();
        }

        //-------------------------------------------------------------------------------------------------

        private void SelectDicomFile(object sender, EventArgs e)
        {
            using (OpenFileDialog open_src_dicom_dialog = new OpenFileDialog())
            {
                open_src_dicom_dialog.Title = @"Select Multi-Frames Dicom";
                open_src_dicom_dialog.Multiselect = false;
                open_src_dicom_dialog.RestoreDirectory = true;
                open_src_dicom_dialog.ReadOnlyChecked = true;

                if (open_src_dicom_dialog.ShowDialog() != DialogResult.OK)
                {
                    return;
                }
                m_source_path_textbox.Text = open_src_dicom_dialog.FileName;
                m_source_file_path = open_src_dicom_dialog.FileName;
            }
        }


        //-------------------------------------------------------------------------------------------------

        private void SelectDestFolder(object sender, EventArgs e)
        {
            using (FolderBrowserDialog open_target_dicom_dialog = new FolderBrowserDialog())
            {
                if (open_target_dicom_dialog.ShowDialog() != DialogResult.OK) {
                    return;
                }
                m_dest_path_textbox.Text = open_target_dicom_dialog.SelectedPath;
                m_destination_foler_path = open_target_dicom_dialog.SelectedPath;
            }
        }

        //-------------------------------------------------------------------------------------------------

        private void UpdateSourcePath(object sender, EventArgs e)
        {
            m_source_file_path = m_source_path_textbox.Text;
            if (m_source_path_textbox.Text.Length <= 0 ||
                m_dest_path_textbox.Text.Length <= 0
            ) {
                m_run_button.Enabled = false;
                m_cancel_button.Enabled = false;
            } else {
                m_run_button.Enabled = true;
                //m_cancel_button.Enabled = true;
            }
        }

        //-------------------------------------------------------------------------------------------------

        private void UpdateTargetPath(object sender, EventArgs e)
        {
            m_destination_foler_path = m_dest_path_textbox.Text;
            if (m_source_path_textbox.Text.Length <= 0 ||
                m_dest_path_textbox.Text.Length <= 0
            ) {
                m_run_button.Enabled = false;
                m_cancel_button.Enabled = false;
            } else {
                m_run_button.Enabled = true;
                //m_cancel_button.Enabled = true;
            }
        }

        //-------------------------------------------------------------------------------------------------

        private void DoRun(object sender, EventArgs e)
        {
            m_progressBar.Value = 0;
            Regex regex = new Regex(@"^([a-zA-Z]):\\?[^\/\:\*\?\""\<\>\|\,]*$");
            Match src_match = regex.Match(m_source_file_path);
            Match dest_match = regex.Match(m_destination_foler_path);
            if (!src_match.Success || !dest_match.Success)
            {
                MessageBox.Show("Invalid path, please input again!");
                return;
            }

            if (m_source_file_path.Length <=0 ||
                m_destination_foler_path.Length <= 0
            ) {
                MessageBox.Show("Invalid input path!");
                return;
            }

            // Clear old data in the destination folder currently (backup is better)
            if (Directory.Exists(m_destination_foler_path))
            {
                DelectDir(m_destination_foler_path);
            }

            m_source_path_textbox.Enabled = false;
            m_dest_path_textbox.Enabled = false;
            m_load_multi_frames_button.Enabled = false;
            m_saved_dest_button.Enabled = false;
            m_run_button.Enabled = false;

            m_progressBar.Value += 50;
            System.Threading.Thread thread1 = new System.Threading.Thread(new System.Threading.ThreadStart(SplitData));
            thread1.Start();
            //thread1.Join();
        }

        //-------------------------------------------------------------------------------------------------

        private void SplitData()
        {
            using (m_dicom_wrapper = new DicomWrapper(DicomWrapper.IOD.eFluoroscopy))
            {
                m_dicom_wrapper.SplitFluoroscopy(m_source_file_path, m_destination_foler_path);
            }

            m_source_path_textbox.Enabled = true;
            m_dest_path_textbox.Enabled = true;
            m_load_multi_frames_button.Enabled = true;
            m_saved_dest_button.Enabled = true;
            m_run_button.Enabled = true;

            m_progressBar.Value += 50;
        }

        //-------------------------------------------------------------------------------------------------

        private void DoCancel(object sender, EventArgs e)
        {
            // Do nothing temporarily
        }

        //-------------------------------------------------------------------------------------------------

        public static void DelectDir(string dest_path)
        {
            try
            {
                DirectoryInfo dir = new DirectoryInfo(dest_path);
                FileSystemInfo[] fileinfo = dir.GetFileSystemInfos();
                foreach (FileSystemInfo file in fileinfo)
                {
                    if (file is DirectoryInfo)
                    {
                        DirectoryInfo subdir = new DirectoryInfo(file.FullName);
                        subdir.Delete(true);
                    }
                    else
                    {
                        File.Delete(file.FullName);
                    }
                }
            }
            catch (Exception e)
            {
                throw;
            }
        }
    }
}
