using System;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using System.Linq;

namespace Anagrams.Tests
{
    [TestClass]
    public class AnagramTest
    {
        [TestMethod]
        [ExpectedException(typeof(ArgumentNullException))]
        public void Constructor_NullAsParameter_ExceptionThrown()
        {
            new Anagram(null);
        }

        [TestMethod]
        public void Constructor_WordAsParameter_KeyDefined()
        {
            String word = "вертикаль";
            var anagram = new Anagram(word);

            Assert.AreEqual(anagram.Key, Anagram.CreateKey(word));
        }

        [TestMethod]
        public void Constructor_WordAsParameter_WordAdded()
        {
            String word = "вертикаль";
            var anagram = new Anagram(word);

            Assert.IsTrue(anagram.Count() == 1 && anagram.Contains(word));
        }

        [TestMethod]
        public void TryAdd_Null_FalseReturned()
        {
            String word = "вертикаль";
            var anagram = new Anagram(word);

            Boolean value = anagram.TryAdd(null);

            Assert.IsFalse(value);
        }

        [TestMethod]
        public void TryAdd_Null_NoChangesMade()
        {
            String word = "вертикаль";
            var anagram = new Anagram(word);

            Boolean value = anagram.TryAdd(null);

            Assert.IsTrue(anagram.Count() == 1 && anagram.Contains(word));
        }

        [TestMethod]
        public void TryAdd_NotAnagram_FalseReturned()
        {
            String word = "вертикаль", wordToAdd = "король";
            var anagram = new Anagram(word);

            Boolean value = anagram.TryAdd(wordToAdd);

            Assert.IsFalse(value);
        }

        [TestMethod]
        public void TryAdd_NotAnagram_NoChangesMade()
        {
            String word = "вертикаль", wordToAdd = "король";
            var anagram = new Anagram(word);

            Boolean value = anagram.TryAdd(wordToAdd);

            Assert.IsTrue(anagram.Count() == 1 && anagram.Contains(word));
        }

        [TestMethod]
        public void TryAdd_ExistingAnagram_FalseReturned()
        {
            String word = "вертикаль";
            var anagram = new Anagram(word);

            Boolean value = anagram.TryAdd(word);

            Assert.IsFalse(value);
        }

        [TestMethod]
        public void TryAdd_ExistingAnagram_NoChangesMade()
        {
            String word = "вертикаль";
            var anagram = new Anagram(word);

            Boolean value = anagram.TryAdd(word);

            Assert.IsTrue(anagram.Count() == 1 && anagram.Contains(word));
        }

        [TestMethod]
        public void TryAdd_NewAnagram_TrueReturned()
        {
            String word = "вертикаль", wordToAdd = "кильватер";
            var anagram = new Anagram(word);

            Boolean value = anagram.TryAdd(wordToAdd);

            Assert.IsTrue(value);
        }

        [TestMethod]
        public void TryAdd_NewAnagram_AnagramAdded()
        {
            String word = "вертикаль", wordToAdd = "кильватер";
            var anagram = new Anagram(word);

            Boolean value = anagram.TryAdd(wordToAdd);

            Assert.IsTrue(anagram.Count() == 2 && anagram.Contains(word) && anagram.Contains(wordToAdd));
        }

        [TestMethod]
        public void CreateKey_ForNull_NullReturned()
        {
            String key = Anagram.CreateKey(null);

            Assert.IsNull(key);
        }

        [TestMethod]
        public void CreateKey_ForWord_NotNullReturned()
        {
            String word = "вертикаль";
            String key = Anagram.CreateKey(word);

            Assert.IsNotNull(key);
        }

        [TestMethod]
        public void IsAnagram_SingleWord_FalseReturned()
        {
            String word = "вертикаль";
            var anagram = new Anagram(word);

            Boolean value = anagram.IsAnagram;

            Assert.IsFalse(value);
        }

        [TestMethod]
        public void IsAnagram_MultipleWords_TrueReturned()
        {
            String word = "вертикаль", wordToAdd = "кильватер";
            var anagram = new Anagram(word);
            anagram.TryAdd(wordToAdd);

            Boolean value = anagram.IsAnagram;

            Assert.IsTrue(value);
        }
    }
}
