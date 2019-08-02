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
    /// <summary>
    /// This class provides the utility functions for the NodeMCU program.
    /// It encomposes the TcpClient from .Net, and added a few functions for networking
    /// related operations.
    /// </summary>
    public static class Helper
    {
        // This client object allows us to do socket operations
        private static TcpClient client = null;

        /// <summary>
        /// Establish TCP connection with the host server
        /// </summary>
        /// <param name="server">The host server's IPv4 address</param>
        /// <param name="port">The port number where the host listen for incoming connections</param>
        /// <returns>true iff a connection has been established</returns>
        public static bool Start(String server, int port)
        {
            try
            {
                client = new TcpClient(server, port);
                return true;
            }
            catch (SocketException e)
            {
                Console.WriteLine(e.ToString());
                return false;
            }
        }

        /// <summary>
        /// Close the established connection, if any
        /// </summary>
        public static void Close()
        {
            if (client != null)
            {
                client.Close();
                client = null;
            }
        }

        /// <summary>
        /// Send a String message to the host if a connection has been established
        /// </summary>
        /// <param name="content">The String to send</param>
        /// <returns>true iff the message has been sent, false otherwise</returns>
        /// <throws>ArgumentNullException if content == null</throws>
        public static bool Send(String content)
        {
            if (client == null)
            {
                return false;
            } else if (content == null)
            {
                throw new ArgumentNullException("content cannot be null");
            }

            // Convert the given content into an array of bytes
            // Notice the \r, this is the end-of-line character, we add this to ease parsing instructions
            // on the ESP8266 module.
            Byte[] bArray = Encoding.ASCII.GetBytes(content + '\r');  // Convert string to bytes
            NetworkStream stream = client.GetStream();
            stream.Write(bArray, 0, bArray.Length);
            stream.Flush();
            return true;
        }

        /// <summary>
        /// Ping the given host for response
        /// </summary>
        /// <param name="nameOrAddress">The host IPv4 address to ping</param>
        /// <returns>true iff the host is online</returns>
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

        /// <summary>
        /// Scans the nearby IPv4 addresses under the same network environment. Note that this functions
        /// does a simple "look-ahead" from the machine's own IPv4 address, so it is not guaranteed that
        /// all online networking devices be discovered.
        /// </summary>
        /// <param name="lst">A list where the result will be stored, each entry is a String, representing the Ipv4 address of the device</param>
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

        /// <summary>
        /// Get the IPv4 address of this machine
        /// </summary>
        /// <returns>The String representation of this machine's IPv4 address, or empty string if offline</returns>
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
