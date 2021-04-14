using System;
using System.Configuration;

namespace todolistserver
{
    class Program
    {
        /// <summary>
        /// 
        /// </summary>
        /// <param name="args"></param>
        static void Main(string[] args)
        {

            if (args.Length > 0)
            {
                var handler = new FileHandler(ConfigurationManager.AppSettings.Get("dbPath"));
                switch (args[0])
                {
                    case "-c":
                        Console.WriteLine(handler.AddArticle(args[1]));
                        handler.SaveChanges();
                        break;
                    case "-r":
                        var articles = handler.GetJsonArticles;
                        Console.WriteLine(articles);
                        break;
                    case "-u":
                        handler.UpdateArticle(int.Parse(args[1]), args[2]);
                        handler.SaveChanges();
                        break;
                    case "-d":
                        handler.DeleteArticle(int.Parse(args[1]));
                        handler.SaveChanges();
                        break;
                }
            }
        }
    }
}
