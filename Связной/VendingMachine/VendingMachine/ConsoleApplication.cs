using System;
using System.Collections.Generic;
using VendingMachine.Actions;
using VendingMachine.Roles;

namespace VendingMachine
{
    class ConsoleApplication
    {
        public MajorTradeAction Main { get; }

        public ConsoleApplication(ICustomer customer, ISeller seller)
        {
            if (customer == null)
                throw new ArgumentNullException(nameof(customer));

            if (seller == null)
                throw new ArgumentNullException(nameof(seller));

            customer.BuyFrom(seller);

            var actions = new List<ITradeAction>
            {
                new PayTradeAction(customer, seller),
                new ChooseProductTradeAction(customer, seller),
                new RequestChangeTradeAction(customer, seller)
            };

            Main = new MajorTradeAction(customer, seller, actions);
        }
    }
}
