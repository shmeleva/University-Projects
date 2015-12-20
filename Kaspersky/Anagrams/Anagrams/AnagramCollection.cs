using System;
using System.Collections;
using System.Collections.Generic;

namespace Anagrams
{
    public class AnagramCollection : IEnumerable<Anagram>
    {
        private readonly List<Anagram> anagrams = new List<Anagram>();

        public Anagram FindAnagram(String key) => anagrams.Find(anagram => anagram.Key == key);

        public void Add(String word)
        {
            if (word == null) return;

            String key = Anagram.CreateKey(word);
            Anagram match = FindAnagram(key);

            if (match == null)
                anagrams.Add(new Anagram(word));
            else
                match.TryAdd(word);
        }

        public IEnumerator<Anagram> GetEnumerator()
        {
            foreach (var anagram in anagrams)
                yield return anagram;
        }

        IEnumerator IEnumerable.GetEnumerator()
        {
            return GetEnumerator();
        }
    }
}
