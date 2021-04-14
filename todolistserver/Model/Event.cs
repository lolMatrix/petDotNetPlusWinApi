using System;

namespace todolistserver.Model
{
    [Serializable]
    class Event
    {
        public int Id { get; set; }
        public string Name { get; set; }
        public long Date { get; set; }
        public long Duration { get; set; }
        public string Location { get; set; }
        public int ImportanceId { get; set; }
        public Period period { get; set; }

        [Serializable]
        public struct Period
        {
            public bool HasPeriod { get; set; }
            public int PeriodId { get; set; }

            private int repeatCount;
            public int RepeatCount
            {
                get => repeatCount;
                set
                {
                    if (value < 2 && value > 366)
                    {
                        Console.WriteLine("Неа");
                    }
                    else
                        repeatCount = value;
                }
            }
        }
    }
}
