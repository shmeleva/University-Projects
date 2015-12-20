using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;

namespace Anagrams
{
    class Program
    {
        static void Main(string[] args)
        {
            #region Первый вариант

            var anagrams = new AnagramCollection();

            using (var sr = new StreamReader(@"Words.txt", Encoding.Default))
                while (sr.EndOfStream != true)
                    anagrams.Add(sr.ReadLine());

            var filtered = anagrams.Where(anagram => anagram.IsAnagram);

            using (var sw = new StreamWriter(@"Anagrams.txt", false))
                foreach (var anagram in filtered)
                    sw.WriteLine(anagram);

            #endregion

            #region Второй вариант

            var words = new List<String>();

            using (var sr = new StreamReader(@"Words.txt", Encoding.Default))
                while (sr.EndOfStream != true)
                    words.Add(sr.ReadLine());

            var anagrams2 = from word in words.Distinct()
                            group word by String.Concat(word.OrderBy(letter => letter)) into grouping
                            where grouping.Count() > 1
                            select grouping;

            using (var sw = new StreamWriter(@"Anagrams (2).txt", false))
                foreach (var grouping in anagrams2)
                    sw.WriteLine(String.Join(", ", grouping));

            #endregion
        }
    }
}