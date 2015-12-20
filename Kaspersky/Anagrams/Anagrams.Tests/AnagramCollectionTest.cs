using System;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using System.Linq;

namespace Anagrams.Tests
{
    [TestClass]
    public class AnagramCollectionTest
    {
        private AnagramCollection anagrams = new AnagramCollection();

        [TestMethod]
        public void Add_WordToEmptyCollection_WordAddedToCreatedList()
        {
            anagrams.Add("равновесие");

            // Содержит одну коллекцию, в которое содержится единственное слово "равновесие".
            Assert.IsTrue(anagrams.Count() == 1 && anagrams.First().Count() == 1 && anagrams.First().Contains("равновесие"));
        }

        [TestMethod]
        public void Add_NonExistentWordToExistingList_WordAdded()
        {
            anagrams.Add("равновесие");
            anagrams.Add("своенравие");

            // Содержит одну коллекцию, в которое содержатся слова "равновесие" и "своенравие".
            Assert.IsTrue(anagrams.Count() == 1 && anagrams.First().Count() == 2 &&
                anagrams.First().Contains("равновесие") && anagrams.First().Contains("своенравие"));
        }

        [TestMethod]
        public void Add_ExistingWord_NoChangesMade()
        {
            anagrams.Add("равновесие");
            anagrams.Add("своенравие");
            anagrams.Add("своенравие");

            // Содержит одну коллекцию, в которое содержатся слова "равновесие" и "своенравие"
            // в единственном экземпляре.
            Assert.IsTrue(anagrams.Count() == 1 && anagrams.First().Count() == 2 &&
                anagrams.First().Contains("равновесие") && anagrams.First().Contains("своенравие"));
        }

        [TestMethod]
        public void Add_NonExistentWordToNewList_WordAddedToCreatedList()
        {
            anagrams.Add("равновесие");
            anagrams.Add("вертикаль");

            // Содержит 2 коллекции, в первой содержится слово "равновесие", во второй - "вертикаль".
            Assert.IsTrue(anagrams.Count() == 2
                && anagrams.First().Count() == 1 && anagrams.First().Contains("равновесие")
                && anagrams.Last().Count() == 1 && anagrams.Last().Contains("вертикаль"));
        }

        [TestMethod]
        public void Add_Null_NoChangesMade()
        {
            anagrams.Add(null);

            // Не содержит коллекций.
            Assert.IsTrue(anagrams.Count() == 0);
        }

        [TestMethod]
        public void FindAnagram_NullAsParameter_NullReturned()
        {
            anagrams.Add("равновесие");
            Anagram found = anagrams.FindAnagram(null);

            // Нет найденных коллекций.
            Assert.IsNull(found);
        }

        [TestMethod]
        public void FindAnagram_ExistingKeyAsParameter_IGroupingReturned()
        {
            anagrams.Add("равновесие");
            String key = Anagram.CreateKey("равновесие");

            Anagram found = anagrams.FindAnagram(key);

            // Возвращена коллекция со словом "равновесие".
            Assert.IsTrue(found != null && found.Contains("равновесие"));
        }

        [TestMethod]
        public void FindAnagram_NonExistentKeyAsParameter_NullReturned()
        {
            anagrams.Add("равновесие");
            String key = Anagram.CreateKey("король");

            Anagram found = anagrams.FindAnagram(key);

            // Нет найденных коллекций.
            Assert.IsNull(found);
        }
    }
}
