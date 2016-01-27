using System;
using System.Collections.Generic;
using System.Linq;
using VendingMachine.Exceptions;

namespace VendingMachine.Products
{
    public class Menu
    {
        private readonly Dictionary<Guid, ProductCollection> _products = new Dictionary<Guid, ProductCollection> ();
        private IEnumerable<KeyValuePair<Guid, ProductCollection>> _avaliable => _products.Where(item => item.Value.IsAvaliable);

        public IEnumerable<Product> Products
        {
            get
            {
                foreach (var product in _products)
                    yield return product.Value.ProductInfo;
            }
        }

        public IEnumerable<Product> AvaliableProducts
        {
            get
            {
                foreach (var product in _avaliable)
                    yield return product.Value.ProductInfo;
            }
        }

        public Menu(IEnumerable<ProductCollection> menu)
        {
            foreach (var item in menu)
                _products.Add(item.ProductInfo.Barcode, item);
        }

        public Product TakeOne(Guid barcode)
        {
            try
            {
                return _products[barcode].TakeOne();
            }
            catch(KeyNotFoundException)
            {
                throw new InvalidBarcodeException(barcode);
            }
        }

        public Int32 GetAmount(Guid barcode)
        {
            try
            {
                return _products[barcode].Amount;
            }
            catch (KeyNotFoundException)
            {
                throw new InvalidBarcodeException(barcode);
            }
        }

        public Int32 GetPrice(Guid barcode)
        {
            try
            {
                return _products[barcode].ProductInfo.Price;
            }
            catch (KeyNotFoundException)
            {
                throw new InvalidBarcodeException(barcode);
            }
        }
    }
}
