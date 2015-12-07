using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DesignPatterns
{
    class Gui
    {
        private Boolean _on;

		private Battle _battle;
		private Int32 _strategyIndex;

        public Gui()
		{
            StartNewBattle();
		}

		private void GameOverSound(String message)
		{
			Console.Beep(300, 500);
		}
        private void SpecialActionSound(String message)
		{
            Console.Beep(400, 50);
		}
		private void DeathSound(Unit dead)
		{
            Console.Beep(200, 50);
		}
        public void Log(String message)
		{
            Console.WriteLine(message);
		}

		public void StartGameCycle()
		{
            Log("New battle.");

            Log(string.Format("Chosen strategy: {0}.", _battle.CurrentStrategy.Name.ToLower()));
            Console.WriteLine();

            ShowField();
            Console.WriteLine();

            while (_on)
			{
                CaptureInput();
			}
		}

        private void CaptureInput()
		{
			Boolean properKey = false;

            while (properKey == false)
			{
				ConsoleKeyInfo keyInfo = Console.ReadKey(true);
                properKey = true;

				switch (keyInfo.Key)
				{
					case ConsoleKey.Escape:
                        Escape();
						break;
					case ConsoleKey.Spacebar:
						Action();
						break;
					case ConsoleKey.LeftArrow:
						Back();
						break;
					case ConsoleKey.RightArrow:
						Forward();
						break;
					case ConsoleKey.DownArrow:
                        ShowAvailableStrategies();
						break;
					case ConsoleKey.R:
                        RestoreBattle();
						break;
                    case ConsoleKey.N:
                        StartNewBattle();
                        break;
					default:
                        properKey = false;
						break;
				}
			}
		}

        private void Escape()
        {
            Log("Peace.");
            Console.ReadKey(true);
            _on = false;
        }

        private void RestoreBattle()
        {
            if (_battle.Field.Status == Battlefield.BattleStatus.Active)
            {
                _battle.Restore();

                Log("Battlefield has been restored.");
                Console.WriteLine();
                ShowField();
                Console.WriteLine();
            }
            else
            {
                Log("This battle is over. But you can start a new one.");
                Console.WriteLine();
            }
        }
        private void StartNewBattle()
        {
            _battle = new Battle();
            _strategyIndex = _battle.AvailableStrategies.FindIndex(strategies => strategies == _battle.CurrentStrategy);
            _on = true;

            _battle.LogEvent += Log;
            _battle.DeathEvent += DeathSound;
            _battle.SpecialActionEvent += SpecialActionSound;
            _battle.GameOverEvent += GameOverSound;

            StartGameCycle();
        }

        private void Action()
        {
            if (_battle.Field.Status == Battlefield.BattleStatus.Active)
            {
                _battle.Action();

                Console.WriteLine();
                ShowField();
                Console.WriteLine();
            }
            else
            {
                Log("Units can't make a move. Battle is finished.");
                Console.WriteLine();
            }
        }
        private void Back()
        {
            if (_battle.Field.Status == Battlefield.BattleStatus.Active)
            {
                if (_battle.MovesDone > 0)
                {
                    _battle.Back();

                    Console.WriteLine();
                    ShowField();
                    Console.WriteLine();
                }
                else
                {
                    Log("Coming back is not an option.");
                    Console.WriteLine();
                }
            }
            else
            {
                Log("The past is already gone. Battle is finished.");
                Console.WriteLine();
            }
        }
        private void Forward()
        {
            if (_battle.Field.Status == Battlefield.BattleStatus.Active)
            {
                if (_battle.MovesUndone > 0)
                {
                    _battle.Forward();

                    Console.WriteLine();
                    ShowField();
                    Console.WriteLine();
                }
                else
                {
                    Log("The future is blurry.");
                    Console.WriteLine();
                }
            }
            else
            {
                Log("There is no future. Battle is finished.");
                Console.WriteLine();
            }
        }

        private void ShowAvailableStrategies()
        {
            Console.ForegroundColor = ConsoleColor.Cyan;
            Console.WriteLine("Available strategies:");

            for (int i = 0; i < _battle.AvailableStrategies.Count; i++)
            {
                if (i == _strategyIndex)
                {
                    Console.ForegroundColor = ConsoleColor.Magenta;
                    Console.WriteLine("{0}. {1}", i + 1, _battle.AvailableStrategies[i].Name);
                    Console.ForegroundColor = ConsoleColor.Cyan;
                }
                else
                {
                    Console.WriteLine("{0}. {1}", i + 1, _battle.AvailableStrategies[i].Name);
                }
            }

            Console.ForegroundColor = ConsoleColor.White;

            Console.WriteLine();

            while (true)
            {
                ConsoleKeyInfo keyInfo = Console.ReadKey(true);

                if (keyInfo.Key == ConsoleKey.Escape)
                    return;


                Int32 digit;
                if (Int32.TryParse(keyInfo.KeyChar.ToString(), out digit))
                {
                    if (digit > 0 && digit <= _battle.AvailableStrategies.Count)
                    {
                        _strategyIndex = digit - 1;
                        _battle.ChangeStrategy(_strategyIndex);
                        Console.ForegroundColor = ConsoleColor.Magenta;
                        Console.WriteLine(string.Format("Chosen strategy: {0}.", _battle.CurrentStrategy.Name.ToLower()));
                        Console.ForegroundColor = ConsoleColor.White;
                        Console.WriteLine();
                        return;
                    }
                }
            }
        }

		private void ShowField()
		{
            Console.ForegroundColor = ConsoleColor.White; ;
            Console.WriteLine("Армия с правом первой атаки: " + _battle.Field.GetArmyName(_battle.Field.CurrentArmy) + ".");

			StringBuilder stringField = new StringBuilder();
            String seporatorString = "    ";

            for (int i = _battle.Field.Reds.Count - 1; i >= 0; i--)
            {
                stringField.Append(_battle.Field.Reds[i].ToString() + " ");
            }

            if (stringField.Length != 0)
                stringField.Append(seporatorString);

            Console.ForegroundColor = ConsoleColor.Red;
            Console.Write(stringField.ToString());

            stringField.Clear();

            for (int i = 0; i < _battle.Field.Whites.Count; i++)
            {
                stringField.Append(_battle.Field.Whites[i].ToString() + " ");
            }
            Console.ForegroundColor = ConsoleColor.White;
            Console.WriteLine(stringField.ToString());
		}
    }
}
