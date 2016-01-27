using System;
using System.Linq;
using VendingMachine.Finance;
using VendingMachine.Roles;

namespace VendingMachine.Actions
{
    class PayTradeAction : TradeAction
    {
        private IOrderedEnumerable<Coin> _coins => _customer.Wallet.Select(s => s.Key).OrderBy(s => s);

        public override string Description => "Place money on deposit";

        public override Action Execute => delegate
        {
            Inform();
            ReadKey(Act, Inform);
        };

        protected override Action Inform => delegate
        {
            WriteHeader(_seller.CustomerDeposit, _customer.WalletBalance);

            Coin[] array = _coins.ToArray();

            for (int i = 0; i < array.Length; i++)
                Console.WriteLine($"[{i + 1}]\t{array[i]}");

            WriteEsc();
        };

        protected override Action<ConsoleKeyInfo> Act => delegate (ConsoleKeyInfo keyInfo)
        {
            int option = keyInfo.KeyChar - '0';
            Coin[] array = _coins.ToArray();

            ThrowNewArgumentOutOfRangeException(option, 1, array.Length);

            _customer.Pay(array[option - 1]);
        };

        public PayTradeAction(ICustomer customer, ISeller seller) : base(customer, seller) { }
    }
}
