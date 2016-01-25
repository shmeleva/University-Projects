using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using VendingMachine.Finance;
using VendingMachine.Products;

namespace VendingMachine.Roles
{
    interface ISeller
    {
        void PutMoney(Coin coin);

        Product GetItem(string barcode);

        IBalance RequestChange();
    }
}
