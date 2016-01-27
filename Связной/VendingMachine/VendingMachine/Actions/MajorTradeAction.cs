using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using VendingMachine.Roles;

namespace VendingMachine.Actions
{
    class MajorTradeAction : TradeAction
    {
        private List<ITradeAction> _actions;

        public override string Description => string.Empty;

        public override Action Execute => delegate
        {
            Inform();
            ReadKey(Act, Inform);
        };

        protected override Action Inform => delegate
        {
            WriteHeader(_seller.CustomerDeposit, _customer.WalletBalance);

            for (int i = 0; i < _actions.Count; i++)
                Console.WriteLine($"[{i + 1}] {_actions[i].Description}");
        };

        protected override Action<ConsoleKeyInfo> Act => delegate (ConsoleKeyInfo keyInfo)
        {
            int option = keyInfo.Key - ConsoleKey.D0;
            ThrowNewArgumentOutOfRangeException(option, 1, _actions.Count);
            _actions[option - 1].Execute();
        };

        public MajorTradeAction(ICustomer customer, ISeller seller, List<ITradeAction> actions) : base(customer, seller)
        {
            if (actions == null)
            {
                throw new NullReferenceException(nameof(actions));
            }

            _actions = actions;
        }
    }
}
