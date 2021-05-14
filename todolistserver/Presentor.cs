using System;
using System.Collections.Generic;
using System.IO;
using System.IO.Pipes;
using System.Text;

namespace todolistserver
{
    class Presentor
    {
        private NamedPipeServerStream stream;
        private FileHandler handler;

        public Presentor(NamedPipeServerStream stream, FileHandler handler)
        {
            this.stream = stream;
            this.handler = handler;
        }

        public void Start()
        {
            stream.WaitForConnection();

            using StreamReader reader = new StreamReader(stream);

            while (stream.IsConnected)
            {
                if (stream.CanRead)
                {
                    string query = reader.ReadLine();
                    byte[] sendingPackege;
                    switch (query)
                    {
                        case "-c":
                            int id = handler.AddArticle(reader.ReadLine());
                            sendingPackege = Encoding.UTF8.GetBytes(id.ToString());
                            stream.Write(sendingPackege, 0, sendingPackege.Length);
                            handler.SaveChanges();
                            break;
                        case "-r":
                            var articles = handler.GetJsonArticles;
                            sendingPackege = Encoding.UTF8.GetBytes(articles);
                            stream.Write(sendingPackege, 0, sendingPackege.Length);
                            break;
                        case "-u":
                            handler.UpdateArticle(int.Parse(reader.ReadLine()), reader.ReadLine());
                            handler.SaveChanges();
                            break;
                        case "-d":
                            handler.DeleteArticle(int.Parse(reader.ReadLine()));
                            handler.SaveChanges();
                            break;
                    }
                }
            }

            stream.Close();
        }


    }
}
