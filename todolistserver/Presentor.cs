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
                string query = reader.ReadLine();

                switch (query)
                {
                    case "-c":
                        Console.WriteLine(handler.AddArticle(reader.ReadLine()));
                        handler.SaveChanges();
                        break;
                    case "-r":
                        var articles = handler.GetJsonArticles;
                        var sendingPackege = Encoding.UTF8.GetBytes(articles);
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


    }
}
