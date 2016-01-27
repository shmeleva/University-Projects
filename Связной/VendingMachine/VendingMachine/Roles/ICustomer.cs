using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using VendingMachine.Finance;
using VendingMachine.Products;

namespace VendingMachine.Roles
{
    interface ICustomer
    {
        IEnumerable<Product> BoughtProducts { get; }
        IDictionary<Coin, int> Wallet { get; }
        int WalletBalance { get; }

        void BuyFrom(ISeller seller);
        void Pay(Coin coin);
        void ChooseProduct(Guid barcode);
        void RequestChange();
    }
}
