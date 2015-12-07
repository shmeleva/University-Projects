using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DesignPatterns
{
    class Battlefield
    {
        public enum BattleStatus
        {
            Active,
            Finished
        }

        private const Int32 armyCost = 2500;

        private List<Unit> _reds;
        private List<Unit> _whites;

        private List<Unit> _currentArmy;

        public List<Unit>  Reds
        {
            get
            {
                return _reds;
            }
        }
        public List<Unit> Whites
        {
            get
            {
                return _whites;
            }
        }

        public List<Unit> CurrentArmy
        {
            get
            {
                return _currentArmy;
            }
        }
        public List<Unit> OppositeArmy
        {
            get
            {
                return GetOppositeArmy(_currentArmy);
            }
        }

        public BattleStatus Status
        {
            get;
            set;
        }

        public Battlefield()
        {
            _reds = ArmyFactoryFacade.Instance.CreateArmy(armyCost);
            _whites = ArmyFactoryFacade.Instance.CreateArmy(armyCost);

            _currentArmy = (BattleRandom.NextBoolean()) ? _reds : _whites;
        }

        public String GetArmyName(List<Unit> team)
        {
            return (team == _reds) ? "Red" : "White";
        }

        public void SetCurrentArmy(List<Unit> army)
        {
            _currentArmy = army;
        }

        public List<Unit> GetOppositeArmy(List<Unit> army)
        {
           return (army == _reds) ? _whites :_reds;
        }

        public void SwitchCurrentArmy()
        {
            _currentArmy = GetOppositeArmy(_currentArmy);
        }

        public List<Unit> FindAllies(Unit unit)
        {
            if (_reds.Contains(unit))
                return _reds;

            if (_whites.Contains(unit))
                return _whites;

            return null;
        }
    }
}
