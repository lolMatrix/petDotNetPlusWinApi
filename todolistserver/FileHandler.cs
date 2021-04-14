using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.Serialization.Formatters.Binary;
using System.Text.Json;
using todolistserver.Model;

namespace todolistserver
{
    class FileHandler
    {
        private List<Event> events;

        public Event GetEvent(int id) => events.FirstOrDefault(x => x.Id == id);

        private string path;

        public FileHandler(string dbPath)
        {
            path = dbPath;
            OpentDb(dbPath);
        }

        private void OpentDb(string path)
        {
            using var file = new FileStream(path, FileMode.OpenOrCreate);

            if (file.Length > 0)
            {
                var formatter = new BinaryFormatter();
                var events = (Event[]) formatter.Deserialize(file);
                this.events = events.ToList();
            }
            else
            {
                events = new List<Event>();
            }
           
        }

        public int AddArticle(string json)
        {
            Event newArticle = JsonSerializer.Deserialize<Event>(json);
            events.Add(newArticle);
            int index = 0;

            if(events.Count > 1)
                index = events.Max(x => x.Id) + 1;

            newArticle.Id = index;
            return index;
        }

        public int DeleteArticle(int id)
        {
            var resultCode = 0;

            int countDeleted = events.RemoveAll(x => x.Id == id);

            if (countDeleted <= 0)
                resultCode = -1;

            return resultCode;
        }

        public int UpdateArticle(int id, string json)
        {
            int resultCode = 0;
            Event updatedEvent = JsonSerializer.Deserialize<Event>(json);
            int countMatch = events.RemoveAll(x => x.Id == id);

            events.Add(updatedEvent);

            return resultCode;
        }

        public string GetJsonArticles => JsonSerializer.Serialize(events.ToArray());

        public void SaveChanges()
        {
            using var database = new FileStream(path, FileMode.Open);
            database.Position = 0;
            var formmater = new BinaryFormatter();
            formmater.Serialize(database, events.ToArray());
        }

        public IEnumerable GetEvent()
        {
            foreach(var e in events)
            {
                yield return e;
            }
        }
    }
}
