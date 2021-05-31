using System.Collections.Generic;
using System.Configuration;
using System.IO.Pipes;
using System.Threading;

namespace todolistserver
{
    class Program
    {
        /// <summary>
        /// Сколько тредов буит создано
        /// </summary>
        private const int numTreads = 10;

        static void Main(string[] args)
        {
            //список тредов
            List<Thread> threads = new List<Thread>();

            //создаем numTreads тредов
            for (int i = 0; i < numTreads; i++)
            {
                var thread = CreatePipe();
                threads.Add(thread);
            }
            //удаляем тред если соеденение в канаве
            while (threads.Count > 0)
            {
                for (int i = threads.Count - 1; i >= 0; i--)
                {

                    if (!threads[i].IsAlive)
                    {
                        threads.Remove(threads[i]);
                        var thread = CreatePipe();
                        threads.Insert(i, thread);
                    }
                }
            }
        }

        /// <summary>
        /// Создает тред с готовым пайпом
        /// </summary>
        /// <returns>тред с пайпом</returns>
        private static Thread CreatePipe()
        {
            var handler = new FileHandler(ConfigurationManager.AppSettings.Get("dbPath"));
            var pipe = new NamedPipeServerStream(ConfigurationManager.AppSettings.Get("pipeName"), PipeDirection.InOut, numTreads);
            var presentor = new Presentor(pipe, handler);

            Thread thread = new Thread(presentor.Start);
            thread.Start();
            return thread;
        }

    }
}
