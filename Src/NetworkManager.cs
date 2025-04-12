using System;
using System.Net;
using System.Net.Sockets;
using System.Threading;
using System.Collections.Generic;
using Akarra.Communication;

namespace Akarra.Network
{
    /// <summary>
    /// Handles network communication for the Akarra client or server
    /// </summary>
    public class NetworkManager
    {
        private const int AkarraPort = 0x4ABC; // Default port
        private const int BufferSize = 8192;   // Default buffer size
        
        private Socket _socket;
        private bool _isServer;
        private byte[] _receiveBuffer;
        private List<NetworkClient> _clients; // For server mode
        
        public bool IsConnected => _socket != null && _socket.Connected;
        
        /// <summary>
        /// Creates a new NetworkManager
        /// </summary>
        /// <param name="isServer">True if this is a server, false for client</param>
        public NetworkManager(bool isServer)
        {
            _isServer = isServer;
            _receiveBuffer = new byte[BufferSize];
            _clients = new List<NetworkClient>();
        }
        
        /// <summary>
        /// Initializes the network manager
        /// </summary>
        public bool Initialize()
        {
            try
            {
                if (_isServer)
                {
                    _socket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
                    _socket.Bind(new IPEndPoint(IPAddress.Any, AkarraPort));
                    _socket.Listen(10);
                    
                    // Start accepting clients
                    _socket.BeginAccept(AcceptCallback, null);
                }
                else
                {
                    _socket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
                }
                
                return true;
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Network initialization failed: {ex.Message}");
                return false;
            }
        }
        
        /// <summary>
        /// Connects to the specified server (client mode only)
        /// </summary>
        public bool Connect(string serverAddress)
        {
            if (_isServer)
                return false;
                
            try
            {
                IPAddress address;
                if (!IPAddress.TryParse(serverAddress, out address))
                {
                    // Try to resolve hostname
                    IPHostEntry hostEntry = Dns.GetHostEntry(serverAddress);
                    if (hostEntry.AddressList.Length == 0)
                        return false;
                        
                    address = hostEntry.AddressList[0];
                }
                
                _socket.Connect(new IPEndPoint(address, AkarraPort));
                
                // Start receiving data
                _socket.BeginReceive(_receiveBuffer, 0, BufferSize, SocketFlags.None, ReceiveCallback, null);
                
                return true;
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Connection failed: {ex.Message}");
                return false;
            }
        }
        
        /// <summary>
        /// Sends a packet to the server (client mode) or to a client (server mode)
        /// </summary>
        public bool SendPacket(byte[] data, NetworkClient client = null)
        {
            try
            {
                if (_isServer)
                {
                    if (client == null)
                        return false;
                        
                    client.Socket.BeginSend(data, 0, data.Length, SocketFlags.None, SendCallback, client);
                }
                else
                {
                    _socket.BeginSend(data, 0, data.Length, SocketFlags.None, SendCallback, null);
                }
                
                return true;
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Send failed: {ex.Message}");
                return false;
            }
        }
        
        /// <summary>
        /// Closes the connection
        /// </summary>
        public void Disconnect()
        {
            if (_socket != null)
            {
                if (_isServer)
                {
                    // Disconnect all clients
                    foreach (NetworkClient client in _clients)
                    {
                        client.Socket.Close();
                    }
                    _clients.Clear();
                }
                
                _socket.Close();
                _socket = null;
            }
        }
        
        /// <summary>
        /// Callback for client connection acceptance (server mode)
        /// </summary>
        private void AcceptCallback(IAsyncResult ar)
        {
            try
            {
                Socket clientSocket = _socket.EndAccept(ar);
                
                NetworkClient client = new NetworkClient
                {
                    Socket = clientSocket,
                    Buffer = new byte[BufferSize],
                    Id = Guid.NewGuid()
                };
                
                _clients.Add(client);
                
                // Start receiving data from this client
                clientSocket.BeginReceive(client.Buffer, 0, BufferSize, SocketFlags.None, 
                                        ReceiveCallback, client);
                
                // Continue accepting clients
                _socket.BeginAccept(AcceptCallback, null);
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Accept failed: {ex.Message}");
            }
        }
        
        /// <summary>
        /// Callback for data reception
        /// </summary>
        private void ReceiveCallback(IAsyncResult ar)
        {
            try
            {
                NetworkClient client = ar.AsyncState as NetworkClient;
                Socket socket = _isServer ? client.Socket : _socket;
                
                int bytesRead = socket.EndReceive(ar);
                
                if (bytesRead > 0)
                {
                    byte[] receivedData = new byte[bytesRead];
                    Buffer.BlockCopy(_isServer ? client.Buffer : _receiveBuffer, 0, receivedData, 0, bytesRead);
                    
                    // Process the received data
                    ProcessReceivedData(receivedData, client);
                    
                    // Continue receiving data
                    socket.BeginReceive(_isServer ? client.Buffer : _receiveBuffer, 0, BufferSize, 
                                       SocketFlags.None, ReceiveCallback, client);
                }
                else
                {
                    // Connection closed
                    if (_isServer)
                    {
                        _clients.Remove(client);
                        client.Socket.Close();
                    }
                    else
                    {
                        _socket.Close();
                        _socket = null;
                    }
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Receive failed: {ex.Message}");
            }
        }
        
        /// <summary>
        /// Callback for data sending
        /// </summary>
        private void SendCallback(IAsyncResult ar)
        {
            try
            {
                NetworkClient client = ar.AsyncState as NetworkClient;
                Socket socket = _isServer ? client.Socket : _socket;
                
                int bytesSent = socket.EndSend(ar);
                
                // Could handle partial sends here, but for simplicity we'll assume complete sends
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Send callback failed: {ex.Message}");
            }
        }
        
        /// <summary>
        /// Processes received data and dispatches appropriate handlers
        /// </summary>
        private void ProcessReceivedData(byte[] data, NetworkClient client)
        {
            // This would parse the GenericPacket header and dispatch to appropriate handler
            // Simplified implementation for demonstration
            if (data.Length < 8) // At least 8 bytes for GenericPacket header
                return;
                
            uint packetType = BitConverter.ToUInt32(data, 0);
            uint packetSize = BitConverter.ToUInt32(data, 4);
            
            // Dispatch to appropriate packet handler based on packetType
            // This would typically be implemented as a delegate or event system
            Console.WriteLine($"Received packet type: {packetType}, size: {packetSize}");
        }
    }
    
    /// <summary>
    /// Represents a client connection for server mode
    /// </summary>
    public class NetworkClient
    {
        public Socket Socket { get; set; }
        public byte[] Buffer { get; set; }
        public Guid Id { get; set; }
        
        // Additional client state could be stored here
    }
}