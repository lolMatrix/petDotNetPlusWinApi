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

        /// <summary>
        /// Получает запись по id
        /// </summary>
        /// <param name="id">id lol</param>
        /// <returns>Запись лол</returns>
        public Event GetEvent(int id) => events.FirstOrDefault(x => x.Id == id);

        private string path;

        public FileHandler(string dbPath)
        {
            path = dbPath;
            OpentDb(dbPath);
        }
        /// <summary>
        /// Получает из файла данные и сохраняет путь
        /// </summary>
        /// <param name="path"></param>
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
        /// <summary>
        /// Добавление записи
        /// </summary>
        /// <param name="json">Сериализованный json'чик</param>
        /// <returns>индекс в бд</returns>
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

        /// <summary>
        /// Удаляет по id запись в бд
        /// </summary>
        /// <param name="id">id lol</param>
        /// <returns>0, если все ок. -1, если все не ок</returns>
        public int DeleteArticle(int id)
        {
            var resultCode = 0;

            int countDeleted = events.RemoveAll(x => x.Id == id);

            if (countDeleted <= 0)
                resultCode = -1;

            return resultCode;
        }
        /// <summary>
        /// обновляет запись в бд по id 
        /// </summary>
        /// <param name="id">id lol</param>
        /// <param name="json">json сериализованная запись</param>
        public void UpdateArticle(int id, string json)
        {
            Event updatedEvent = JsonSerializer.Deserialize<Event>(json);
            int countMatch = events.RemoveAll(x => x.Id == id);

            events.Add(updatedEvent);
        }

        /// <summary>
        /// Получает все записи в формате json
        /// </summary>
        public string GetJsonArticles => JsonSerializer.Serialize(events.ToArray());
        /// <summary>
        /// сохраняет изменения в файл
        /// </summary>
        public void SaveChanges()
        {
            using var database = new FileStream(path, FileMode.Open);
            database.Position = 0;
            var formmater = new BinaryFormatter();
            formmater.Serialize(database, events.ToArray());
        }

        /// <summary>
        /// красивый енамчик, который переберает все все все записи
        /// </summary>
        /// <returns>одну запись</returns>
        public IEnumerable GetEvent()
        {
            foreach(var e in events)
            {
                yield return e;
            }
        }
    }
}
