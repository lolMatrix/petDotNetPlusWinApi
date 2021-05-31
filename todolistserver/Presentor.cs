using System;
using System.Collections.Generic;
using System.IO;
using System.IO.Pipes;
using System.Text;

namespace todolistserver
{
    class Presentor
    {
        /// <summary>
        /// сама пайпа
        /// </summary>
        private NamedPipeServerStream stream;
        /// <summary>
        /// то, что работает с файлом
        /// </summary>
        private FileHandler handler;

        public Presentor(NamedPipeServerStream stream, FileHandler handler)
        {
            this.stream = stream;
            this.handler = handler;
        }

        /// <summary>
        /// Начальный метод. Запускает стрим и ждет данные
        /// </summary>
        public void Start()
        {
            stream.WaitForConnection();

            Console.WriteLine("Client connected");

            using StreamReader reader = new StreamReader(stream);

            while (stream.IsConnected)
            {
                if (stream.CanRead)
                {
                    Console.WriteLine("Read data from client");
                    string query = reader.ReadLine();
                    byte[] sendingPackege;
                    switch (query)
                    {
                        case "-c":
                            Console.WriteLine("Create");
                            lock (handler)//цэ пятая лаба ака блок ресурсов
                            {
                                Console.WriteLine("Waiting for creating article...");
                                int id = handler.AddArticle(reader.ReadLine());
                                handler.SaveChanges();
                                sendingPackege = Encoding.UTF8.GetBytes(id.ToString());
                                stream.Write(sendingPackege, 0, sendingPackege.Length);
                                Console.WriteLine("Article has been created");
                            }
                            break;
                        case "-r":
                            Console.WriteLine("read");
                            lock (handler)
                            {
                                var articles = handler.GetJsonArticles;
                                sendingPackege = Encoding.UTF8.GetBytes(articles);
                                stream.Write(sendingPackege, 0, sendingPackege.Length);
                            }
                            break;
                        case "-u":
                            Console.WriteLine("Update");
                            lock (handler)
                            {
                                handler.UpdateArticle(int.Parse(reader.ReadLine()), reader.ReadLine());
                                handler.SaveChanges();
                            }
                            break;
                        case "-d":
                            Console.WriteLine("delete");
                            lock (handler)
                            {
                                handler.DeleteArticle(int.Parse(reader.ReadLine()));
                                handler.SaveChanges();
                            }
                            break;
                    }
                }
            }

            Console.WriteLine("Connection closed");

            stream.Close();
        }


    }
}
