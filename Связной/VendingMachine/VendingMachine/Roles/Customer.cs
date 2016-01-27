using System;
using System.Collections.Generic;
using VendingMachine.Products;
using VendingMachine.Finance;

namespace VendingMachine.Roles
{
    class Customer : ICustomer
    {
        private readonly List<Product> _boughtProducts = new List<Product>();
        private readonly IBalance _customerBalance;

        private ISeller _seller;

        public int Balance => _customerBalance.Overall;

        public IEnumerable<Product> BoughtProducts
        {
            get
            {
                foreach (var product in _boughtProducts)
                    yield return product;
            }
        }

        public IDictionary<Coin, int> Wallet =>
            _customerBalance.StacksOfCoins;

        public int WalletBalance => _customerBalance.Overall;

        public Customer(IBalance customerBalance)
        {
            if (customerBalance == null)
                throw new ArgumentNullException();

            _customerBalance = customerBalance;
        }

        public void BuyFrom(ISeller seller)
        {
            _seller = seller;
        }

        public void Pay(Coin coin)
        {
            ThrowNewInvalidOperationExceptionOnNullSeller();
            _customerBalance.TakeCoin(coin);
            _seller.AcceptPayment(coin);
        }

        public void ChooseProduct(Guid barcode)
        {
            ThrowNewInvalidOperationExceptionOnNullSeller();
            _boughtProducts.Add(_seller.DispenceProduct(barcode));
        }

        public void RequestChange()
        {
            ThrowNewInvalidOperationExceptionOnNullSeller();
            _customerBalance.PutMoney(_seller.ReturnDeposit());
        }

        private void ThrowNewInvalidOperationExceptionOnNullSeller()
        {
            if (_seller == null)
                throw new InvalidOperationException("Не выбран продавец.");
        }
    }
}
