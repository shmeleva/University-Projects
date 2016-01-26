using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using VendingMachine.Finance;
using VendingMachine.Products;

namespace VendingMachine
{
    static class ProjectDefaults
    {
        public static Menu Menu
        {
            get
            {
                var menu = new List<ProductCollection>
                {
                    new ProductCollection(new Product(Guid.NewGuid(), "Cupcake", 50), 4),
                    new ProductCollection(new Product(Guid.NewGuid(), "Cookie", 10), 3),
                    new ProductCollection(new Product(Guid.NewGuid(), "Waffle", 30), 10)
                };

                return new Menu(menu);
            }
        }

        public static VendingMachineBalance VendingMachineBalance
        {
            get
            {
                var balance = new Dictionary<Coin, int>
                {
                    { Coin.OneRuble, 10 },
                    { Coin.TwoRubles, 10 },
                    { Coin.FiveRubles, 5 },
                    { Coin.TenRubles, 5 },
                };

                return new VendingMachineBalance(new Balance(balance));
            }
        }

        public static Balance CustomerBalance
        {
            get
            {
                var balance = new Dictionary<Coin, int>
                {
                    { Coin.OneRuble, 20 },
                    { Coin.TwoRubles, 15 },
                    { Coin.FiveRubles, 10 },
                    { Coin.TenRubles, 5 },
                };

                return new Balance(balance);
            }
        }
    }
}
