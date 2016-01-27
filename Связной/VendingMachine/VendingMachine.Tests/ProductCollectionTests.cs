using System;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using VendingMachine.Products;
using VendingMachine.Exceptions;

namespace VendingMachine.Tests
{
    [TestClass]
    public class ProductCollectionTests
    {
        [TestMethod]
        [ExpectedException(typeof(ArgumentNullException))]
        public void Constructor_NullProductInfo_ExceptionThrown()
        {
            new ProductCollection(null, 10);
        }

        [TestMethod]
        [ExpectedException(typeof(ArgumentException))]
        public void Constructor_LessThanZeroAmount_ExceptionThrown()
        {
            var info = new Product(Guid.NewGuid(), "Cookie", 10);

            new ProductCollection(info, -10);
        }

        [TestMethod]
        public void Constructor_ValidArguments_InstanceCreated()
        {
            var info = new Product(Guid.NewGuid(), "Cookie", 10);
            var amount = 10;

            var instance = new ProductCollection(info, amount);

            Assert.AreEqual(instance.ProductInfo, info);
            Assert.AreEqual(instance.Amount, amount);
        }

        [TestMethod]
        public void IsAvaliableProperty_ZeroAmount_FalseReturned()
        {
            var info = new Product(Guid.NewGuid(), "Cookie", 10);
            var amount = 0;

            var instance = new ProductCollection(info, amount);

            Assert.IsFalse(instance.IsAvaliable);
        }

        [TestMethod]
        public void IsAvaliableProperty_NonZeroAmount_TrueReturned()
        {
            var info = new Product(Guid.NewGuid(), "Cookie", 10);
            var amount = 10;

            var instance = new ProductCollection(info, amount);

            Assert.IsTrue(instance.IsAvaliable);
        }

        [TestMethod]
        public void TakeOne_IsAvaliable_ProductReturned()
        {
            var info = new Product(Guid.NewGuid(), "Cookie", 10);
            var amount = 10;
            var instance = new ProductCollection(info, amount);

            var taken = instance.TakeOne();

            Assert.AreEqual(info.Barcode, taken.Barcode);
            Assert.AreEqual(info.Name, taken.Name);
            Assert.AreEqual(info.Price, taken.Price);
        }

        [TestMethod]
        [ExpectedException(typeof(OutOfStockException))]
        public void TakeOne_NotAvaliable_ExceptionThrown()
        {
            var info = new Product(Guid.NewGuid(), "Cookie", 10);
            var amount = 0;
            var instance = new ProductCollection(info, amount);

            var taken = instance.TakeOne();
        }
    }
}
