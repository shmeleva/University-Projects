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
        int CustomerDeposit { get; }
        IEnumerable<Product> VarietyOfProducts { get; }

        void AcceptPayment(Coin coin);
        Product DispenceProduct(Guid barcode);
        IBalance ReturnDeposit();
    }
}
