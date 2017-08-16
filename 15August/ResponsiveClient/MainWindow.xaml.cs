using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Threading;
using System.Net.Sockets;
using managedcode;
namespace ResponsiveClient
 
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private TcpClient _client { get; set; }
        private Thread _readThread { get; set; }
        int len;
        MessageTypeManaged mt;
        string _sender =null;
        string recipient= null;
        string buffer;
        string username, password;
        RequestManaged _req;
        public MainWindow() { 
            _client = new TcpClient("127.0.0.1", 50009);
            _req = new RequestManaged();
            _readThread = new Thread(new ThreadStart(read));
            _readThread.Start();
            if (_client.Connected)
            {
                MessageBox.Show("Client Connected");
            }
        
            InitializeComponent();
        }
        private void read()
        {
            byte[] _buffer;
            int _type;
            int _size;
            string _payload;
            ResponseManaged resp = new ResponseManaged();
          
                _buffer = new byte[1];
                _client.GetStream().Read(_buffer, 0, 1);
                 _type = Convert.ToInt32(Encoding.ASCII.GetString(_buffer));
                switch ((ResponseMessageManaged)_type)
                {
                 case ResponseMessageManaged.RegisterOK:
                    _buffer = new byte[4];
                    _client.GetStream().Read(_buffer, 0, 4);
                    _size = Convert.ToInt32(Encoding.ASCII.GetString(_buffer));
                    _buffer = new byte[_size];
                    _client.GetStream().Read(_buffer, 0, _size);
                    _payload = Encoding.ASCII.GetString(_buffer);

                    break;


  
                }
            
        }
        private void lblRegister_Click(object sender, RoutedEventArgs e)
        {
            wrapper.SelectedItem = loginScreen;
        }
        private void lblLogin_Click(object sender, RoutedEventArgs e)
        {

            wrapper.SelectedItem = loginScreen;
        }
        private void btnRegister_Click(object sender, RoutedEventArgs e)
        {
            mt = (MessageTypeManaged)(Convert.ToInt32(btnRegister.Uid));
            len = 21;
            username = txtUsername.Text;
            password = txtPassword.Text;
            buffer = username + password;
            _req.SetFields(mt, _sender, recipient, buffer, len);
            RequestHelperManaged.SendRequest(_client, _req.ToString());
        }
        private void btnLogin_Click(object sender, RoutedEventArgs e)
        {
            mt = (MessageTypeManaged)(Convert.ToInt32(btnRegister.Uid));
            len = 21;
            username = txtUsername.Text;
            password = txtPassword.Text;
            buffer = username + password;
            _req.SetFields(mt, _sender, recipient, buffer, len);
            RequestHelperManaged.SendRequest(_client, _req.ToString());
        }

     
       
    }
}
