using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using VendingMachine.Exceptions;
using VendingMachine.Finance;
using VendingMachine.Products;
using VendingMachine.Roles;

namespace VendingMachine
{
    class ConsoleApplication
    {
        private ICustomer _customer;
        private ISeller _seller;

        public ConsoleApplication(ICustomer customer, ISeller seller)
        {
            if (customer == null)
                throw new ArgumentNullException("Customer is null.");

            if (seller == null)
                throw new ArgumentNullException("Customer is null.");

            _customer = customer;
            _seller = seller;

            customer.BuyFrom(seller);
        }

        public void Run()
        {
            Main();
        }

        #region Main

        private void Main()
        {
            MainInfo();
            MainAction();
        }

        private void MainInfo()
        {
            Console.Clear();

            DepositInfo();

            Console.WriteLine();

            foreach (var product in _seller.VarietyOfProducts)
                Console.WriteLine($"{product.Name}\t{product.Price}RUB");

            Console.WriteLine();

            Console.WriteLine("[1] Внести деньги");
            Console.WriteLine("[2] Выбрать продукт");
            Console.WriteLine("[3] Вернуть сдачу");
        }

        private void MainAction()
        {
            bool properKey = false;

            while (properKey == false)
            {
                ConsoleKeyInfo keyInfo = Console.ReadKey(true);
                properKey = true;

                switch (keyInfo.Key)
                {
                    case ConsoleKey.D1:
                        Pay();
                        break;
                    case ConsoleKey.D2:
                        ChooseProduct();
                        break;
                    case ConsoleKey.D3:
                        RequestChange();
                        break;
                    default:
                        properKey = false;
                        break;
                }
            }
        }

        #endregion

        #region Pay

        private void Pay()
        {
            var coins = _customer.Wallet.Select(s => s.Key).OrderBy(s => s).ToArray();

            PayInfo(coins);
            PayAction(coins);

            Main();
        }

        private void PayInfo(Coin[] coins)
        {
            Console.Clear();

            DepositInfo();

            Console.WriteLine();

            if (coins.Length != 0)
            {
                for (int i = 0; i < coins.Length; i++)
                    Console.WriteLine($"[{i + 1}]\t{coins[i]}");

                Console.WriteLine("[ESC]");

                Console.WriteLine();
            }

            WalletInfo();
        }

        private void PayAction(Coin[] coins)
        {
            bool properKey = false;

            while (properKey == false)
            {
                ConsoleKeyInfo keyInfo = Console.ReadKey(true);
                properKey = true;

                switch (keyInfo.Key)
                {
                    case ConsoleKey.Escape:
                        return;
                    default:
                        int option = keyInfo.Key - ConsoleKey.D0;

                        if (InRange(option, 1, coins.Length))
                        {
                            _customer.Pay(coins[option - 1]);
                            break;
                        }

                        properKey = false;
                        break;
                }
            }

            Pay();
        }

        #endregion

        #region ChooseProduct

        private void ChooseProduct()
        {
            var products = _seller.VarietyOfProducts.ToArray();

            ChooseProductInfo(products);

            try
            {
                ChooseProductAction(products);
            }
            catch(NotEnoughMoneyVendingMachineException e)
            {
                Console.Clear();
                DepositInfo();
                Console.WriteLine();
                Console.WriteLine(e.Message);
                Escape();
            }

            Main();
        }

        private void ChooseProductInfo(Product[] products)
        {
            Console.Clear();

            DepositInfo();

            Console.WriteLine();

            for (int i = 0; i < products.Length; i++)
                Console.WriteLine($"[{i + 1}]\t{products[i].Name}\t{products[i].Price}RUB");

            Console.WriteLine("[ESC]");

            Console.WriteLine();
        }

        private void ChooseProductAction(Product[] products)
        {
            bool properKey = false;

            while (properKey == false)
            {
                ConsoleKeyInfo keyInfo = Console.ReadKey(true);
                properKey = true;

                switch (keyInfo.Key)
                {
                    case ConsoleKey.Escape:
                        return;
                    default:
                        int option = keyInfo.Key - ConsoleKey.D0;

                        if (InRange(option, 1, products.Length))
                        {
                            _customer.ChooseProduct(products[option - 1].Barcode);
                            LastBoughtProduct();
                            return;
                        }

                        properKey = false;
                        break;
                }
            }
        }

        private void LastBoughtProduct()
        {
            Console.Clear();
            DepositInfo();
            Console.WriteLine();
            Console.WriteLine($"Заберите \"{_customer.BoughtProducts.Last().Name}\"");
            Escape();
        }

        #endregion

        #region RequestChange

        private void RequestChange()
        {
            Console.Clear();
            try
            {
                _customer.RequestChange();
                Console.WriteLine("Возьмите сдачу");
            }
            catch (NoChangeVendingMachineException e)
            {
                Console.WriteLine(e.Message);
            }

            Escape();
            Main();
        }

        #endregion

        private void DepositInfo()
        {
            Console.WriteLine($"Внесенная сумма: [{_seller.CustomerDeposit}]");
        }

        private void WalletInfo()
        {
            int total = _customer.Wallet.Sum(s => (int)s.Key * s.Value);
            Console.WriteLine($"Осталось: [{total}]");
        }

        private bool InRange(int option, int lower, int upper)
        {
            return (option >= lower) && (option <= upper);
        }

        private void Escape()
        {
            Console.WriteLine("[ESC]");

            bool properKey = false;

            while (properKey == false)
            {
                ConsoleKeyInfo keyInfo = Console.ReadKey(true);
                properKey = true;

                switch (keyInfo.Key)
                {
                    case ConsoleKey.Escape:
                        return;
                    default:
                        properKey = false;
                        break;
                }
            }
        }
    }
}
