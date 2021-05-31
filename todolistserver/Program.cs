using System;
using System.Configuration;
using System.IO.Pipes;
using System.Threading;

namespace todolistserver
{
    class Program
    {
        static void Main(string[] args)
        {
            var handler = new FileHandler(ConfigurationManager.AppSettings.Get("dbPath"));

            var pipe = new NamedPipeServerStream(ConfigurationManager.AppSettings.Get("pipeName"));

            var presentor = new Presentor(pipe, handler);

            Thread da = new Thread(presentor.Start);
            da.Start();
        }

        
    }
}
