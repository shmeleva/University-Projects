using System;
using System.Linq;
using VendingMachine.Roles;
using VendingMachine.Exceptions;
using VendingMachine.Products;

namespace VendingMachine.Actions
{
    class ChooseProductTradeAction : TradeAction
    {
        private Product[] _products;

        public override string Description => "Buy some delicious snacks";

        public override Action Execute => delegate
        {
            _products = _seller.VarietyOfProducts.ToArray();

            Inform();
            ReadKey(Act, null, false);
        };

        protected override Action Inform => delegate
        {
            WriteHeader(_seller.CustomerDeposit, _customer.WalletBalance);

            for (int i = 0; i < _products.Length; i++)
                Console.WriteLine($"[{i + 1}]\t{_products[i].Name}\t{_products[i].Price}RUB");

            WriteEsc();
        };

        protected override Action<ConsoleKeyInfo> Act => delegate(ConsoleKeyInfo keyInfo)
        {
            int option = keyInfo.KeyChar - '0';

            ThrowNewArgumentOutOfRangeException(option, 1, _products.Length);

            try
            {
                _customer.ChooseProduct(_products[option - 1].Barcode);

                WriteHeader(_seller.CustomerDeposit, _customer.WalletBalance);
                Console.WriteLine($"Here is your \"{_customer.BoughtProducts.Last().Name}\"");
            }
            catch(InsufficientFundsException e)
            {
                WriteHeader(_seller.CustomerDeposit, _customer.WalletBalance);
                Console.WriteLine(e.Message);
            }

            WriteEsc();
        };

        public ChooseProductTradeAction(ICustomer customer, ISeller seller) : base(customer, seller) { }
    }
}
