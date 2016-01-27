using System;
using VendingMachine.Exceptions;

namespace VendingMachine.Products
{
    public class ProductCollection
    {
        public Product ProductInfo { get; }
        public int Amount { get; private set; }

        public Boolean IsAvaliable => (Amount > 0);

        public ProductCollection(Product info, int amount)
        {
            if (info == null)
            {
                throw new ArgumentNullException(nameof(info));
            }

            if (amount < 0)
            {
                throw new ArgumentException(nameof(amount));
            }

            ProductInfo = info;
            Amount = amount;
        }

        public ProductCollection(Guid barcode, string name, int price, int amount) :
            this (new Product(barcode, name, price), amount) { }

        public Product TakeOne()
        {
            if (IsAvaliable == false)
            {
                throw new OutOfStockException(ProductInfo.Name);
            }

            Amount--;
            return ProductInfo.Clone();
        }
    }
}
