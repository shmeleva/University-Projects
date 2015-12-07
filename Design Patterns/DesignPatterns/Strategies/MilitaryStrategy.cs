using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DesignPatterns
{
    abstract class MilitaryStrategy
    {
        public abstract String Name { get; }

        protected readonly CommandManager _commandManager;
        protected readonly Battlefield _field;

        protected MilitaryStrategy(CommandManager commandManager, Battlefield field)
		{
			_commandManager = commandManager;
            _field = field;
		}

        protected void ExecuteNewCommand(Command command)
        {
            _commandManager.ExecuteNewCommand(command);
        }

        protected abstract List<Unit> GetTargets(ISpecialAction specialUnit, List<Unit> allies, List<Unit> enemies);
        protected abstract void RunSpecialActions(List<Unit> army);

        public abstract void AttackPhase();
        public abstract void SpecialActionPhase();
    }
}
