using System;
using System.Linq;
using VendingMachine.Finance;
using VendingMachine.Roles;

namespace VendingMachine.Actions
{
    class PayTradeAction : TradeAction
    {
        private Coin[] _coins;

        public override string Description => "Place money on deposit";

        public override Action Execute => delegate
        {
            _coins = _customer.Wallet.Select(s => s.Key).OrderBy(s => s).ToArray();

            Inform();
            ReadKey(Act, Inform);
        };

        protected override Action Inform => delegate
        {
            WriteHeader(_seller.CustomerDeposit, _customer.WalletBalance);

            for (int i = 0; i < _coins.Length; i++)
                Console.WriteLine($"[{i + 1}]\t{_coins[i]}");

            WriteEsc();
        };

        protected override Action<ConsoleKeyInfo> Act => delegate (ConsoleKeyInfo keyInfo)
        {
            int option = keyInfo.Key - ConsoleKey.D0;

            ThrowNewArgumentOutOfRangeException(option, 1, _coins.Length);

            _customer.Pay(_coins[option - 1]);
        };

        public PayTradeAction(ICustomer customer, ISeller seller) : base(customer, seller) { }
    }
}
