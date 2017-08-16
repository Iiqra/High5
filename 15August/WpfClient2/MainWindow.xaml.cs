using System;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using System.Windows;


namespace WpfClient2
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private TcpClient _client { get; set; }
        private Thread readThread { get; set; }

        public MainWindow()
        {
            InitializeComponent();

            // _client = new TcpClient(IPAddress.Loopback.ToString(), 50009);

             _client = new TcpClient("127.0.0.1", 50009);
              readThread = new Thread(new ThreadStart(read));
            // readThread.Start();
        }

        private async void write(string content)
        {
            if (_client.Connected)
            { 
                //001 1 byte 0-255// '1' alt 001   // 1uaername34password67 --0001000 0000000001
                var buffer = Encoding.ASCII.GetBytes(content);
                await _client.GetStream().WriteAsync(buffer, 0, buffer.Length);
            }
            else
            {
                MessageBox.Show($"Client not connected. Tried sending \"{content}\".");
            }
        }

        private async void read()
        {
            while(true)
            {
                // Just keep reading.
                byte[] buffer = new byte[1];
                await _client.GetStream().ReadAsync(buffer, 0, 1);

                // 16 -> 0x10 (16, 0) -> \u0010  1
                var str = Encoding.ASCII.GetString(buffer);
                int type = Convert.ToInt32(str);
                
                // Read further
                buffer = new byte[4];
                await _client.GetStream().ReadAsync(buffer, 0, 4);

               var strr=  Encoding.ASCII.GetString(buffer);
                int length = Convert.ToInt32(strr);
                buffer = new byte[length];
                await _client.GetStream().ReadAsync(buffer, 0, length);

                // Payload
                MessageBox.Show(Encoding.ASCII.GetString(buffer));
            }
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            // _client.GetStream().Write(null, 0, 0);
        }

        private void btnregister_Click(object sender, RoutedEventArgs e)
        {
            string content = "";
            if(rg1.IsChecked == true)
            {
                content = rg1.Content as string;
            } else if (rg2.IsChecked == true)
            {
                content = rg2.Content as string;
            }
            else if (rg3.IsChecked == true)
            {
                content = rg3.Content as string;
            }
            else if (rg4.IsChecked == true)
            {
                content = rg4.Content as string;
            }
            else if (rgc.IsChecked == true)
            {
                content = rgc.Content as string;
            }

            // Send the request to the server.
            write(content);
        }

        private void btnlogin_Click(object sender, RoutedEventArgs e)
        {
            wrapper.SelectedItem = login;
        }

        private void btnregister_Click_1(object sender, RoutedEventArgs e)
        {

        }
    }
}
