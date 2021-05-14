using System.Collections.Generic;
using System.Configuration;
using System.IO.Pipes;
using System.Threading;

namespace todolistserver
{
    class Program
    {
        private const int numTreads = 10;

        /// <summary>
        /// 
        /// </summary>
        /// <param name="args"></param>
        static void Main(string[] args)
        {
            

            List<Thread> threads = new List<Thread>();


            for (int i = 0; i < numTreads; i++)
            {
                var thread = CreatePipe();
                threads.Add(thread);
            }

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
