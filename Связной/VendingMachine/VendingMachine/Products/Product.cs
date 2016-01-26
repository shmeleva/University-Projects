using System;
using System.Diagnostics.Contracts;

namespace VendingMachine.Products
{
    public class Product : ICloneable, IEquatable<Product>
    {
        public Guid Barcode { get; }
        public string Name { get; }
        public int Price { get; }

        public Product(Guid barcode, string name, int price)
        {
            if (barcode == default(Guid))
                throw new ArgumentException("Barcode is not specified.");

            if (string.IsNullOrEmpty(name))
                throw new ArgumentException("Product name is not specified.");

            if (price < 0)
                throw new ArgumentException("Price is less than zero.");

            Barcode = barcode;
            Name = name;
            Price = price;
        }

        #region ICloneable

        public Product Clone() =>
            new Product(Barcode, Name, Price);

        object ICloneable.Clone() =>
            Clone();

        #endregion

        #region IEquatable<Product>

        public override Boolean Equals(object obj) =>
            (obj == null) ? false : this.Equals(obj as Product);

        public override int GetHashCode() =>
            new { Barcode, Name, Price }.GetHashCode();

        public Boolean Equals(Product item) =>
            (item == null) ? false : (item.Barcode == Barcode) && (item.Name == Name) && (item.Price == Price);

        #endregion
    }
}
