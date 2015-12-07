using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DesignPatterns
{
    class ThreeOnThreeStrategy : MilitaryStrategy
    {
        private const Int32 _three = 3;

        public override String Name
        {
            get
            {
                return "Три на трех";
            }
        }

        public ThreeOnThreeStrategy(CommandManager commandManager, Battlefield field)
            : base(commandManager, field) { }


        public override void AttackPhase()
        {
            if (_field.Reds.Count == 0 || _field.Whites.Count == 0)
                throw new Exception("Army of the dead.");

            List<Command> commandHistory = new List<Command>();

            for (int i = 0; i < Math.Min(Math.Min(_field.CurrentArmy.Count, _field.OppositeArmy.Count), _three); i++)
            {
                UnitKeeper firstTarget = _field.CurrentArmy[i].AttackTarget(_field.OppositeArmy[i]);
                firstTarget.Log.ObjectColor = _field.GetArmyName(_field.OppositeArmy);
                firstTarget.Log.SubjectColor = _field.GetArmyName(_field.CurrentArmy);
                commandHistory.Add(new FightCommand(firstTarget, _field));

                if (firstTarget.Changed.Health != 0)
                {
                    UnitKeeper secondTarget = _field.OppositeArmy[i].AttackTarget(_field.CurrentArmy[i]);
                    secondTarget.Log.ObjectColor = _field.GetArmyName(_field.CurrentArmy);
                    secondTarget.Log.SubjectColor = _field.GetArmyName(_field.OppositeArmy);
                    commandHistory.Add(new FightCommand(secondTarget, _field));
                }
            }

            foreach (Command command in commandHistory)
                ExecuteNewCommand(command);
        }

        public override void SpecialActionPhase()
        {
            RunSpecialActions(_field.CurrentArmy);
            RunSpecialActions(_field.OppositeArmy);
        }

        protected override void RunSpecialActions(List<Unit> army)
        {
            if (army.Count < 7)
                return;

            for (int i = 6; i < Math.Min(army.Count, 9); i++)
            {
                ISpecialAction specialUnit = army[i] as ISpecialAction;

                if (specialUnit != null)
                {
                    if (specialUnit.Probability > BattleRandom.Next(100))
                    {
                        List<Unit> targets = GetTargets(specialUnit, army, _field.GetOppositeArmy(army));

                        if (targets.Count != 0)
                        {
                            Int32 targetIndex = BattleRandom.Next(targets.Count);

                            UnitKeeper specialActionTarget = specialUnit.SpecialAction(targets[targetIndex]);
                            specialActionTarget.Log.ObjectColor = _field.GetArmyName(_field.FindAllies(targets[targetIndex]));
                            specialActionTarget.Log.SubjectColor = _field.GetArmyName(_field.FindAllies(specialUnit as Unit));
                            ExecuteNewCommand(new SpecialActionCommand(specialActionTarget, _field));
                        }
                    }
                }
            }
        }

        protected override List<Unit> GetTargets(ISpecialAction specialUnit, List<Unit> allies, List<Unit> enemies)
        {
            Int32 specialUnitIndex = allies.FindIndex(unit => unit == specialUnit as Unit);
            List<Unit> targets = new List<Unit>();

            if (specialUnit.Target == ActionTarget.Any || specialUnit.Target == ActionTarget.Ally)
            {
                for (int i = 0; i < allies.Count; i++)
                {
                    if (GetAllyMaxDistance(specialUnitIndex, i) <= specialUnit.Range && specialUnit.IsCompatibleType(allies[i]))
                    {
                        targets.Add(allies[i]);
                    }
                }
            }

            if (specialUnit.Target == ActionTarget.Any || specialUnit.Target == ActionTarget.Enemy)
            {
                for (int i = 0; i < enemies.Count; i++)
                {
                    if (GetEnemyMaxDistance(specialUnitIndex, i) <= specialUnit.Range && specialUnit.IsCompatibleType(enemies[i]))
                    {
                        targets.Add(enemies[i]);
                    }
                }
            }

            return targets;
        }

        protected Int32 GetAllyMaxDistance(Int32 centerIndex, Int32 circleIndex)
        {
            Int32 xDistance = Math.Abs(GetX(centerIndex) - GetX(circleIndex));
            Int32 yDistance = Math.Abs(GetY(centerIndex) - GetY(circleIndex));

            return Math.Max(xDistance, yDistance);
        }
        protected Int32 GetEnemyMaxDistance(Int32 centerIndex, Int32 circleIndex)
        {
            Int32 xDistance = Math.Abs(GetX(centerIndex) - GetX(circleIndex));
            Int32 yDistance = Math.Abs(GetY(centerIndex) + GetY(circleIndex) + 1);

            return Math.Max(xDistance, yDistance);
        }

        protected Int32 GetX(Int32 index)
        {
            return index % _three;
        }
        protected Int32 GetY(Int32 index)
        {
            return index / _three;
        }
    }
}
