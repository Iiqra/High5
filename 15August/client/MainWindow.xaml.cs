using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Net.Sockets;
using managedcode;

namespace client
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private TcpClient _client { get; set; }
        public MainWindow()
        {
            InitializeComponent();
            _client = new TcpClient("127.0.0.1", 50009);

            if (_client.Connected)
            {
                MessageBox.Show("Client Connected");
            }
        }

       
        private void txtrequest_TextChanged(object sender, TextChangedEventArgs e)
        {
        }

        private void btnenter_Click(object sender, RoutedEventArgs e)
        {


            string content= txtrequest.Text;
            managedcode.RequestHelperManaged.SendRequest(_client, content);  // req object 
            var response = ResponseHelper.ReadResponse(_client);
         
            
            //content = txtrequest.Text;
            //byte[] buffer = Encoding.ASCII.GetBytes(content);
            //_client.GetStream().Write(buffer, 0, buffer.Length);
            //byte[] buffer2 = new byte[12];
            //_client.GetStream().Read(buffer2, 0, 12);
            //txtresponse.Text = Encoding.ASCII.GetString(buffer2);
            //var response = ResponseHelper.ReadResponse(_client);
            //response.ToString();
            //int a = 21;
        }

    }
}
