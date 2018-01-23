using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace DicomToolkit
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);

            try
            {
                Application.Run(new Mainform());
            }
            catch (Exception e)
            {
                ShowExitDialog(e);
            }
        }

        //-----------------------------------------------------------------------------------------

        private static void ShowExitDialog(Exception e) {

            try {
                string error_msg = @"An application error has occurred with the following message:";
                error_msg += System.Environment.NewLine;
                error_msg += System.Environment.NewLine;
                error_msg += @"""" + e.Message + @"""";
                error_msg += System.Environment.NewLine;
                error_msg += System.Environment.NewLine;
                error_msg += @"The application will now close.";

                //using (GenericWarningDialog dlg = new GenericWarningDialog()) {
                //    dlg.MessageText = error_msg;
                //    dlg.FontPointSize = 14;
                //    dlg.TitleText = "DicomToolkit Failure";
                //    dlg.ShowDialog(null);
                //}
            }
            finally {
                Application.Exit();
            }
        }
    }
}
