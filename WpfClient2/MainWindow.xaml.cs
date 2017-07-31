using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

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
        }

        private async void write(string content)
        {
            if (_client.Connected)
            {
                var buffer = Encoding.ASCII.GetBytes(content);
                await _client.GetStream().WriteAsync(buffer, 0, buffer.Length);
                MessageBox.Show($"Client connected. Tried sending \"{content}\" in byte[] of size {buffer.Length}.");
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

                if(BitConverter.ToInt32(buffer, 0) == 1)
                {
                    buffer = new byte[4];
                    await _client.GetStream().ReadAsync(buffer, 0, 4);

                    int length = BitConverter.ToInt32(buffer, 0);
                    buffer = new byte[length];

                    await _client.GetStream().ReadAsync(buffer, 0, length);

                    // Payload
                    MessageBox.Show(BitConverter.ToString(buffer));
                } else 
                {
                    MessageBox.Show("Initial was not a 1.");
                }
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
    }
}
