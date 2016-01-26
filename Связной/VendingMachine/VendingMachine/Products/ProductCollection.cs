using System;
using VendingMachine.Exceptions;

namespace VendingMachine.Products
{
    public class ProductCollection
    {
        public Product ProductInfo { get; }
        public int Amount { get; private set; }

        public Boolean IsAvaliable => (Amount != 0);

        public ProductCollection(Product info, int amount)
        {
            if (info == null)
                throw new ArgumentNullException("Product description is null.");

            if (amount < 0)
                throw new ArgumentException("Product amount is less than zero.");

            ProductInfo = info;
            Amount = amount;
        }

        public ProductCollection(Guid barcode, string name, int price, int amount) :
            this (new Product(barcode, name, price), amount) { }

        public Product TakeOne()
        {
            if (IsAvaliable == false)
                throw new NoItemsLeftVendingMachineException();

            Amount--;
            return ProductInfo.Clone();
        }
    }
}
