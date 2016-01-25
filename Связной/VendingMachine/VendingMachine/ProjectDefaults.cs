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
        public static Menu DefaultMenu
        {
            get
            {
                IEnumerable<ProductCollection> draft = new List<ProductCollection>
                {
                    new ProductCollection(new Product(Guid.NewGuid(), "Cupcake", 50), 4),
                    new ProductCollection(new Product(Guid.NewGuid(), "Cookie", 10), 3),
                    new ProductCollection(new Product(Guid.NewGuid(), "Waffle", 30), 10)
                };

                return new Menu(draft);
            }
        }

        // Временно.
        public static IBalance DefaultVendingMachineBalance
        {
            get
            {
                var one = Enumerable.Repeat(Coin.OneRuble, 5);
                var two = Enumerable.Repeat(Coin.TwoRubles, 5);
                var five = Enumerable.Repeat(Coin.FiveRubles, 5);
                var ten = Enumerable.Repeat(Coin.TwoRubles, 5);

                var coins = new List<Coin>();

                return null;
            }
        }

        public static IBalance DefaultCustomerBalance
        {
            get
            {
                var coins = new List<Coin>();

                coins.AddRange(coins);
                coins.AddRange(coins);
                coins.AddRange(coins);

                var balance = new Balance(coins);

                return balance;
            }
        }
    }
}
