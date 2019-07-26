using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Net;
using System.Net.Sockets;
using System.Net.NetworkInformation;
namespace WindowsFormsApp1
{
    public static class Helper
    {
        private static TcpClient client = null;
        public static bool Start(String server, int port)
        {
            try
            {
                client = new TcpClient(server, port);
                return true;
            }
            catch (SocketException ex)
            {
                return false;
            }
        }

        public static void Close()
        {
            if (client != null)
            {
                client.Close();
                client = null;
            }
        }

        public static bool Send(String content)
        {
            if (client == null)
            {
                return false;
            } else if (content == null)
            {
                throw new ArgumentNullException("content cannot be null");
            }

            Byte[] bArray = Encoding.ASCII.GetBytes(content + '\r');
            NetworkStream stream = client.GetStream();
            stream.Write(bArray, 0, bArray.Length);
            stream.Flush();
            return true;
        }

        public static bool PingHost(string nameOrAddress)
        {
            bool pingable = false;
            Ping pinger = null;

            try
            {
                pinger = new Ping();
                PingReply reply = pinger.Send(nameOrAddress, 1);
                pingable = reply.Status == IPStatus.Success;
            }
            catch (PingException)
            {
                // Discard PingExceptions and return false;
            }
            finally
            {
                if (pinger != null)
                {
                    pinger.Dispose();
                }
            }

            return pingable;
        }

        public static void ScanNearby(List<String> lst)
        {
            String ip = GetLocalIPV4();
            
            //List < String > lst = new List<String>();
            String[] ip_parts;
            int Start;
            int oldStart;
            if (ip.Length == 0)
            {
                return;
            }
            ip_parts = ip.Split('.');
            if (ip_parts.Length < 3)
            {
                return;
            }
            Start = int.Parse(ip_parts[3]);
            oldStart = Start;
            for (Start++; Start <= oldStart + 10; Start++)
            {
                String tmpIP = ip_parts[0] + "." + ip_parts[1] + "." + ip_parts[2] + "." + Start.ToString();
                if (PingHost(tmpIP))
                {
                    lst.Add(tmpIP);
                }
            }
        }

        public static String GetLocalIPV4()
        {
            string localIP = "";
            using (Socket socket = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, 0))
            {
                socket.Connect("8.8.8.8", 65530);
                IPEndPoint endPoint = socket.LocalEndPoint as IPEndPoint;
                localIP = endPoint.Address.ToString();
            }
            return localIP;
        }
    }
}
