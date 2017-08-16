using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using managedcode;
using System.Net.Sockets;
namespace client
{
  public class ResponseHelper
    {
        public static ResponseManaged ReadResponse(TcpClient client)
        { 
            int type = 0;
            var response = new ResponseManaged();
            byte[] buffer = new byte[1];
            client.GetStream().Read(buffer, 0, 1); // read type
            type = Convert.ToInt32(Encoding.ASCII.GetString(buffer));
            if (type == (int)ResponseMessageManaged.CantLogin)
             {
                // Stream is flushed.
                client.GetStream().Read(new byte[15], 0, 15);
            }
            else if (type == (int)ResponseMessageManaged.MemberList)
            {
                buffer = new byte[4];
                client.GetStream().Read(buffer, 0, 4);

                int size = Convert.ToInt32(Encoding.ASCII.GetString(buffer));

                buffer = new byte[size];
                client.GetStream().Read(buffer, 0, size);
            }

            return response;
        }
    }
}
