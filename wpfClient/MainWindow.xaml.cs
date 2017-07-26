using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
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

namespace wpfClient
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

            _client = new TcpClient(IPAddress.Loopback.ToString(), 50009);
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            _client.GetStream().Write(null, 0, 0);
        }
    }
}
