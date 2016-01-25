using System;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using VendingMachine.Products;

namespace VendingMachine.Tests
{
    [TestClass]
    public class ProductTests
    {
        [TestMethod]
        [ExpectedException(typeof(ArgumentException))]
        public void Constructor_DefaultBarcode_ExceptionThrown()
        {
            new Product(default(Guid), "Cookie", 10);
        }

        [TestMethod]
        [ExpectedException(typeof(ArgumentException))]
        public void Constructor_NullName_ExceptionThrown()
        {
            new Product(Guid.NewGuid(), null, 10);
        }

        [TestMethod]
        [ExpectedException(typeof(ArgumentException))]
        public void Constructor_EmptyName_ExceptionThrown()
        {
            new Product(Guid.NewGuid(), "", 10);
        }

        [TestMethod]
        [ExpectedException(typeof(ArgumentException))]
        public void Constructor_PriceLessThanZero_ExceptionThrown()
        {
            new Product(Guid.NewGuid(), "Cookie", -10);
        }

        [TestMethod]
        public void Constructor_ValidArguments_InstanceCreated()
        {
            Guid barcode = Guid.NewGuid();
            string name = "Cookie";
            int price = 10;

            var instance = new Product(barcode, name, price);

            Assert.AreEqual(instance.Barcode, barcode);
            Assert.AreEqual(instance.Name, name);
            Assert.AreEqual(instance.Price, price);
        }

        [TestMethod]
        public void Clone_Cloned()
        {
            Guid barcode = Guid.NewGuid();
            string name = "Cookie";
            int price = 10;

            var instance = new Product(barcode, name, price);
            var cloned = instance.Clone();

            Assert.AreEqual(instance.Barcode, cloned.Barcode);
            Assert.AreEqual(instance.Name, cloned.Name);
            Assert.AreEqual(instance.Price, cloned.Price);
        }
    }
}
