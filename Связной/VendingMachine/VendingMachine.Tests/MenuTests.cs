using System;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using VendingMachine.Products;
using VendingMachine.Exceptions;
using System.Collections;
using System.Collections.Generic;

namespace VendingMachine.Tests
{
    [TestClass]
    public class MenuTests
    {
        [TestMethod]
        [ExpectedException(typeof(InvalidBarcodeException))]
        public void TakeOne_NonExistingBarcode_ExceptionThrown()
        {
            IEnumerable<ProductCollection> draft = new List<ProductCollection>
            {
                new ProductCollection(new Product(Guid.NewGuid(), "Cupcake", 50), 4),
                new ProductCollection(new Product(Guid.NewGuid(), "Cookie", 10), 3),
                new ProductCollection(new Product(Guid.NewGuid(), "Waffle", 30), 10)
            };

            Menu menu = new Menu(draft);

            menu.TakeOne(Guid.NewGuid());
        }

        [TestMethod]
        [ExpectedException(typeof(OutOfStockException))]
        public void TakeOne_NoItemsLeft_ExceptionThrown()
        {
            Guid waffleGuid = Guid.NewGuid();
            Int32 waffleAmount = 0;

            IEnumerable<ProductCollection> draft = new List<ProductCollection>
            {
                new ProductCollection(new Product(Guid.NewGuid(), "Cupcake", 50), 0),
                new ProductCollection(new Product(Guid.NewGuid(), "Cookie", 10), 3),
                new ProductCollection(new Product(waffleGuid, "Waffle", 30), waffleAmount)
            };

            Menu menu = new Menu(draft);

            menu.TakeOne(waffleGuid);
        }

        [TestMethod]
        public void TakeOne_ExistingBarcode_AmountDecreased()
        {
            Guid waffleGuid = Guid.NewGuid();
            Int32 waffleAmount = 10;

            IEnumerable<ProductCollection> draft = new List<ProductCollection>
            {
                new ProductCollection(new Product(Guid.NewGuid(), "Cupcake", 50), 4),
                new ProductCollection(new Product(Guid.NewGuid(), "Cookie", 10), 3),
                new ProductCollection(new Product(waffleGuid, "Waffle", 30), waffleAmount)
            };

            Menu menu = new Menu(draft);

            Product waffle = menu.TakeOne(waffleGuid);

            Assert.AreEqual(menu.GetAmount(waffleGuid), waffleAmount - 1);
        }

        [TestMethod]
        public void TakeOne_ExistingBarcode_RightItemReturned()
        {
            Guid waffleGuid = Guid.NewGuid();

            IEnumerable<ProductCollection> draft = new List<ProductCollection>
            {
                new ProductCollection(new Product(Guid.NewGuid(), "Cupcake", 50), 4),
                new ProductCollection(new Product(Guid.NewGuid(), "Cookie", 10), 3),
                new ProductCollection(new Product(waffleGuid, "Waffle", 30), 10)
            };

            Menu menu = new Menu(draft);

            Product waffle = menu.TakeOne(waffleGuid);

            Assert.AreEqual(waffle.Barcode, waffleGuid);
        }
    }
}
