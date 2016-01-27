using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using VendingMachine.Exceptions;
using VendingMachine.Roles;

namespace VendingMachine.Actions
{
    class RequestChangeTradeAction : TradeAction
    {
        public override string Description => "Request change";

        public override Action Execute => delegate
        {
            Act(default(ConsoleKeyInfo));
            ReadKey(null, null);
        };

        protected override Action Inform => delegate { };

        protected override Action<ConsoleKeyInfo> Act => delegate(ConsoleKeyInfo keyInfo)
        {
            try
            {
                _customer.RequestChange();
                WriteHeader(_seller.CustomerDeposit, _customer.WalletBalance);
                Console.WriteLine($"*coins jingling*{Environment.NewLine}");
            }
            catch (NoChangeVendingMachineException e)
            {
                WriteHeader(_seller.CustomerDeposit, _customer.WalletBalance);
                Console.WriteLine(e.Message);
            }

            WriteEsc();
        };

        public RequestChangeTradeAction(ICustomer customer, ISeller seller) : base(customer, seller) { }
    }
}
