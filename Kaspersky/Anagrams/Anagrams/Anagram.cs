using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;

namespace Anagrams
{
    public class Anagram : IGrouping<string, string>
    {
        private readonly List<String> words = new List<String>();

        public String Key { get; }

        public Boolean IsAnagram
        {
            get
            {
                return words.Count > 1;
            }
        }

        public Anagram(String word)
        {
            if (word == null)
                throw new ArgumentNullException();

            Key = CreateKey(word);
            words.Add(word);
        }

        public Boolean TryAdd(String word)
        {
            // Если null / слово, не являющееся анаграммой / уже добавленное в коллекцию слово.
            if (word == null || CreateKey(word) != Key || words.Find(x => x == word) != null)
                return false;

            words.Add(word);
            return true;
        }

        public static String CreateKey(String word) => word == null? null : String.Concat(word.OrderBy(letter => letter));

        public override String ToString() => String.Join(", ", words);

        public IEnumerator<string> GetEnumerator()
        {
            foreach (var word in words)
                yield return word;
        }

        IEnumerator IEnumerable.GetEnumerator()
        {
            return GetEnumerator();
        }
    }
}
